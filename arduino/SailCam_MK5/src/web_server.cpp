
#include <web_server.h>

#define html_response_overhead 100  // number of characters to allocate per data node to html code, not super efficient but gets the job done.

WebServer::WebServer(HardwareDrivers* hardware_drivers, CommandParser* command_parser)
{
    this->hardware_drivers = hardware_drivers;
    this->command_parser = command_parser;
    server = new ESP8266WebServer(web_server_port);

    html_response = (char *) malloc((serial_buffer_length + html_response_overhead) * sizeof(char));

    server->on("/", HTTP_GET, [this]() {
        handle_root();
    });

    server->on("/api", HTTP_GET, [this]() {
        this->api_parser();
    });

    server->on("/stream", HTTP_GET, [this]() {
        this->stream_camera();
    });

    server->onNotFound([this]() {
        server->send(404, "text/html", "Page not found.");
    });
    server->begin();
}

WebServer::~WebServer()
{
    server->stop();
    free(html_response);
    html_response = NULL;
}

void WebServer::handle_client()
{
    server->handleClient();
}

void WebServer::format_html_response() 
{
    int buffer_index = sprintf(html_response, "<html><head><meta http-equiv='refresh' content='1'></head><body>");

    buffer_index += sprintf(&html_response[buffer_index], "</body></html>");
    html_response[buffer_index] = '\0';
}

void WebServer::handle_root()
{
    format_html_response();
    server->send(200, "text/html",  html_response);
}

void WebServer::api_parser()
{
    char* html = (char*) malloc(sizeof(char) * command_message_buffer_length);
    this->command_parser->process_web_api(server->arg("command").c_str(), server->arg("arg").c_str(), server->arg("param").c_str(), &html);
    server->send(200, "text/html", html);
    free(html);
}

void WebServer::stream_camera()
{
    WiFiClient client = server->client();
  
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    server->sendContent(response);
  
    while (1) {

        hardware_drivers->camera->cam->clear_fifo_flag();
        hardware_drivers->camera->cam->start_capture();

        while (!hardware_drivers->camera->cam->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));

        size_t len = hardware_drivers->camera->cam->read_fifo_length();
        if (len >= MAX_FIFO_SIZE) {
            continue;
        } else if (len == 0 ){
            continue;
        }

        hardware_drivers->camera->cam->CS_LOW();
        hardware_drivers->camera->cam->set_fifo_burst(); 

        #if !(defined (OV5642_MINI_5MP_PLUS) ||(defined (ARDUCAM_SHIELD_V2) && defined (OV5642_CAM)))
        SPI.transfer(0xFF);
        #endif   

        if (!client.connected()) break;

        response = "--frame\r\n";
        response += "Content-Type: image/jpeg\r\n\r\n";
        server->sendContent(response);
        
        static const size_t bufferSize = 4096;
        static uint8_t buffer[bufferSize] = {0xFF};
        
        while (len) {
            size_t will_copy = (len < bufferSize) ? len : bufferSize;
            hardware_drivers->camera->cam->transferBytes(&buffer[0], &buffer[0], will_copy);

            if (!client.connected()) break;

            client.write(&buffer[0], will_copy);
            len -= will_copy;
        }
        hardware_drivers->camera->cam->CS_HIGH();

        if (!client.connected()) break;
    }
}