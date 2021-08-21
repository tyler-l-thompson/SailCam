
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <config.h>
#include <hardware/drivers.h>
#include <util/command_parser.h>
#include <ESP8266WebServer.h>

class WebServer
{

private:
    HardwareDrivers* hardware_drivers;
    CommandParser* command_parser;
    ESP8266WebServer* server;
    char* html_response;
    void format_html_response();
    void handle_root();
    void api_parser();


public:
    WebServer(HardwareDrivers* hardware_drivers, CommandParser* command_parser);
    ~WebServer();
    void allocate_response_memory(int buffer_width);
    void handle_client();

};

#endif
