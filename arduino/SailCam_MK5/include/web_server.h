
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <config.h>
#include <hardware/drivers.h>
#include <util/command_parser.h>
#include <ESP8266WebServer.h>

#define html_response_overhead 1024  // number of characters to allocate per data node to html code, not super efficient but gets the job done.

#define html_header "<html> <head> <link rel='stylesheet' type='text/css' href='/api?command=cat&arg=/html/styles.css'/> </head> <body> <div class='main_text'>"

#define html_footer "</div> <div class='footer'> <table> <tr> <td><a class='footer_link' href='/api?command=cat&arg=/html/controls.html'>Controls</a></td> <td><a class='footer_link' href='/api?command=cat&arg=/html/diagnostics.html'>Diagnostics</a></td> <td><a class='footer_link' href='/api?command=cat&arg=/html/settings.html'>Settings</a></td> </tr> </table> </div> </body> </html>"

class WebServer
{

private:
    HardwareDrivers* hardware_drivers;
    CommandParser* command_parser;
    ESP8266WebServer* server;
    char* html_response;
    void handle_root();
    void api_parser();
    void capture_camera();
    void file_browser();


public:
    WebServer(HardwareDrivers* hardware_drivers, CommandParser* command_parser);
    ~WebServer();
    void allocate_response_memory(int buffer_width);
    void handle_client();

};

#endif
