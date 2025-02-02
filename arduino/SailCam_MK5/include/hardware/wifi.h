
#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <config.h>

enum WIFI_MODE { WIFI_ACCESS_POINT, WIFI_CLIENT, WIFI_DISABLED };

enum TCP_SERVER_MODE { TCP_SERVER_DISABLED, TCP_SERVER_ENABLED };

class Wifi
{
private:
    WIFI_MODE wifi_mode;
    TCP_SERVER_MODE tcp_server_mode;
    const char* wifi_statuses[7];
    char mac_str[18];
    WiFiClient* wifi_client;
    WiFiServer* wifi_server;
    char* api_buffer;
    int api_read_size;
    bool api_new_data;

public:
    Wifi(const char* ssid, const char* psk, int tcp_port, WIFI_MODE wifi_mode, TCP_SERVER_MODE tcp_server_mode);
    ~Wifi();
    WIFI_MODE get_wifi_mode() { return this->wifi_mode; };
    TCP_SERVER_MODE get_tcp_mode() { return this->tcp_server_mode; };
    IPAddress get_ip_address();
    char* get_mac_address();
    const char* get_connection_status_str();
    wl_status_t get_connection_status();
    void start_server(int port);
    void send_server_data(char* data, int size);
    void read_server_data();
    char* get_server_data();
    bool new_data_available();
    bool is_client_connected();
};

#endif
