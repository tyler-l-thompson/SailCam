
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
