
#include <web_server.h>

WebServer::WebServer(HardwareDrivers* hardware_drivers, CommandParser* command_parser)
{
    this->hardware_drivers = hardware_drivers;
    this->command_parser = command_parser;
    this->server = new ESP8266WebServer(web_server_port);

    // this->html_response = (char *) malloc(command_message_buffer_length + html_response_overhead);

    this->server->on("/", HTTP_GET, [this]() {
        handle_root();
    });

    this->server->on("/api", HTTP_GET, [this]() {
        this->api_parser();
    });

    this->server->on("/capture", HTTP_GET, [this]() {
        this->capture_camera();
    });

    this->server->on("/file_browser", HTTP_GET, [this]() {
        this->file_browser();
    });

    this->server->onNotFound([this]() {
        this->server->send(404, "text/html", "Page not found.");
    });
    this->server->begin();
}

WebServer::~WebServer()
{
    this->server->stop();
    // free(html_response);
}

void WebServer::handle_client()
{
    this->server->handleClient();
}

void WebServer::handle_root()
{
    if (!this->hardware_drivers->camera->is_capture_done())
    {
        this->server->send(200, "text/html",  "Camera capture in progress...");
        return;
    }

    this->server->sendHeader("Location", String(root_html_file), true);
    this->server->send(302, "text/plain", "");
}

void WebServer::api_parser()
{
    if (!this->hardware_drivers->camera->is_capture_done())
    {
        this->server->send(200, "text/html", "Camera capture in progress...");
    }
    else
    {
        this->html_response = (char *) malloc(get_safe_buffer_size(command_message_buffer_length + html_response_overhead));
        this->command_parser->process_web_api(this->server->arg("command").c_str(), this->server->arg("arg").c_str(), this->server->arg("param").c_str(), &(this->html_response));
        this->server->setContentLength(strlen(html_header) + strlen(this->html_response) + strlen(html_footer));
        this->server->send(200, "text/html", "");
        this->server->sendContent(html_header);
        this->server->sendContent(this->html_response);
        this->server->sendContent(html_footer);
        free(this->html_response);
    }
}

void WebServer::capture_camera()
{
    uint32_t len;
    uint32_t to_transfer;
    uint32_t buff_len = get_safe_buffer_size(max_buffer_size);
    uint8_t* buffer = (uint8_t*) malloc(buff_len);

    this->hardware_drivers->old_display->clear();
    this->hardware_drivers->old_display->print(oled_capture_message);

    this->server->sendContent("HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n--frame\r\nContent-Type: image/jpeg\r\n\r\n");
  
    this->hardware_drivers->camera->set_sensor_power(true);
    this->hardware_drivers->camera->cam->CS_LOW();
    this->hardware_drivers->camera->cam->clear_fifo_flag();
    this->hardware_drivers->camera->cam->start_capture();

    while (!this->hardware_drivers->camera->cam->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) {
        delay(1);
    }
    
    len = this->hardware_drivers->camera->cam->read_fifo_length();
    this->hardware_drivers->camera->cam->CS_HIGH();

    this->server->setContentLength(buff_len);
    
    if (!this->server->client().connected()) 
    {
        free(buffer);
        return;
    }

    while (len > 0)
    {
        to_transfer = (len < buff_len) ? len : buff_len;
        
        this->hardware_drivers->camera->cam->CS_LOW();
        this->hardware_drivers->camera->cam->set_fifo_burst();
        this->hardware_drivers->camera->cam->transferBytes(buffer, buffer, to_transfer);
        this->hardware_drivers->camera->cam->CS_HIGH();

        if (!this->server->client().connected())
        {
            break;
        }

        this->server->client().write(buffer, to_transfer);
        len -= to_transfer;
    }
    free(buffer);
    this->hardware_drivers->camera->set_sensor_power(false);

    this->hardware_drivers->old_display->clear();
    this->hardware_drivers->old_display->wake_up();
}

void WebServer::file_browser()
{
    uint32_t buff_len = get_safe_buffer_size(max_buffer_size);
    char* buff;
    sdfat::File32 file;

    const char* file_name = this->server->arg("file").c_str();

    /* check if camera capture in progress */
    if (!this->hardware_drivers->camera->is_capture_done())
    {
        this->server->send(200, "text/html", "Camera capture in progress...");
        return;
    }

    /* check if sd card not connected */
    if (!this->hardware_drivers->storage_controller->is_card_connected())
    {
        this->server->send(200, "text/html", "SD card not connected.");
        return;
    }

    /* redirect root to images */
    if (strcmp(file_name, "") == 0)
    {
        file_name = "/";
    }

    /* check if file exists */
    if (!this->hardware_drivers->storage_controller->file_exists(file_name))
    {
        this->server->send(200, "text/html", "File not found.");
        return;
    }

    /* open file/directory and allocate buffer memory */
    file = this->hardware_drivers->storage_controller->open_file(file_name);
    buff = (char *) malloc(buff_len);
    this->hardware_drivers->serial_term->debug_printf("file_browser - buff_len %d free_mem %d\r\n", buff_len, system_get_free_heap_size());

    /* handle directory */
    if (file.isDirectory())
    {
        char link_root[50];
        snprintf(link_root, 50, "/file_browser?file=%s", file_name);
        int len = this->hardware_drivers->storage_controller->list_directory(file, &buff, buff_len, 0, "", "<br>", true, link_root);
        this->server->setContentLength(strlen(html_header) + len + strlen(html_footer));
        this->server->send(200, "text/html", "");
        this->server->sendContent(html_header);
        this->server->sendContent(buff, len);
        this->server->sendContent(html_footer);
    }
    else /* handle file */
    {
        uint32_t file_size = file.size();
        bool is_image = (strcmp(&(file_name[strlen(file_name)-4]), ".jpg") == 0); 

        if (is_image)
        {
            this->server->setContentLength(file_size);
            this->server->send(200, "image/jpg", "");
        }
        else
        {
            this->server->setContentLength(strlen(html_header) + file_size + strlen(html_footer));
            this->server->send(200, "text/html", "");
            this->server->sendContent(html_header);
        }
        
        uint32_t to_transfer = (buff_len > file_size) ? file_size : buff_len;
        for (uint32_t i = 0; i < file_size; i+=to_transfer)
        {
            file.readBytes(buff, to_transfer);

            if ((i + to_transfer) >= file_size)
            {
                buff[to_transfer-1] = '\0';
            }

            this->server->client().write(buff, to_transfer);

            if (!this->server->client().connected()) 
            {
                i = file_size;
            }

            if ((i + to_transfer) > file_size)
            {
                to_transfer = (file_size - i);
            }
        }

        if (!is_image)
        {
            this->server->sendContent(html_footer);
        }
    }
    
    /* free buffer memory and close file */
    free(buff);
    file.close();
}
