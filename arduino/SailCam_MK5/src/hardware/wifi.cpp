
#include <hardware/wifi.h>

Wifi::Wifi(const char* ssid, const char* psk, int tcp_port, WIFI_MODE wifi_mode, TCP_SERVER_MODE tcp_server_mode)
: wifi_statuses
{
    "Idle",
    "No SSID Available",
    "Scan Complete",
    "Connected",
    "Connection Failed",
    "Connection Lost",
    "Disconnected"
}
{
    this->wifi_mode = wifi_mode;
    this->tcp_server_mode = tcp_server_mode;
    this->wifi_client = NULL;
    this->wifi_server = NULL;
    this->api_new_data = false;

    if (wifi_mode == WIFI_ACCESS_POINT) {
        IPAddress local_ip(ap_local_ip);
        IPAddress gateway(ap_gateway_address);
        IPAddress subnet_mask(ap_subnet_mask);
        WiFi.disconnect();
        WiFi.softAPConfig(local_ip, gateway, subnet_mask);
        WiFi.softAP(ssid, psk);
    } else if (wifi_mode == WIFI_CLIENT) {
        WiFi.softAPdisconnect(true);
        WiFi.begin(ssid, psk);
    } else {
        WiFi.disconnect();
        WiFi.softAPdisconnect(true);
    }

    // store the MAC address as a string
    uint8_t mac[6];
    wifi_get_macaddr(STATION_IF, mac);
    sprintf(this->mac_str, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // start tcp server if it is enabled
    if (((wifi_mode == WIFI_ACCESS_POINT) || (wifi_mode == WIFI_CLIENT)) && (tcp_server_mode == TCP_SERVER_ENABLED))
    {
        this->start_server(tcp_port);
    }
}

Wifi::~Wifi()
{
}

IPAddress Wifi::get_ip_address()
{
    if (this->wifi_mode == WIFI_ACCESS_POINT) {
        return WiFi.softAPIP();
    } else {
        return WiFi.localIP();
    }
}

char* Wifi::get_mac_address()
{
    return this->mac_str;
}

const char* Wifi::get_connection_status_str()
{
    if (this->wifi_mode == WIFI_ACCESS_POINT) {
        return "Access Point Mode";
    } else if (this->wifi_mode == WIFI_DISABLED) {
        return "Disabled";
    } else {
        return wifi_statuses[WiFi.status()];
    }
}

wl_status_t Wifi::get_connection_status()
{
    return WiFi.status();
}

void Wifi::start_server(int port)
{
    this->wifi_server = new WiFiServer(port);
    this->wifi_client = new WiFiClient();
    this->wifi_client->setSync(true);
    this->wifi_client->setDefaultSync(true);
    this->wifi_client->setNoDelay(true);
    this->wifi_server->begin();
}

void Wifi::send_server_data(char* data, int size)
{
    if (this->wifi_client && this->wifi_client->connected()) {
        char* data_buf = (char*) malloc(size + 3);
        snprintf(data_buf, size + 3, "%s\r\n", data);
        this->wifi_client->flush();
        this->wifi_client->write(data_buf);
        this->wifi_client->flush();
        free(data_buf);
    }
}

void Wifi::read_server_data()
{
    if (this->is_client_connected() && this->wifi_client->available() > 0) {
        this->api_read_size = this->wifi_client->readBytesUntil('\n', this->api_buffer, serial_buffer_length);
        if (this->api_read_size > serial_min_read && isalnum(this->api_buffer[0])) { // ignore invalid messages
            this->api_buffer[this->api_read_size >= serial_buffer_length ? serial_buffer_length : this->api_read_size] = '\0';  // null terminate string read from buffer
            if (this->api_buffer[this->api_read_size - 1] == '\r') {
                this->api_buffer[this->api_read_size - 1] = '\0';
            }
            this->api_new_data = true;
        }
        this->wifi_client->flush();
    }
}

char* Wifi::get_server_data()
{
    return api_buffer;
}

bool Wifi::new_data_available()
{
    bool return_val = this->api_new_data;
    this->api_new_data = false;
    return return_val;
}

bool Wifi::is_client_connected()
{
    if (this->wifi_mode == WIFI_DISABLED) {
        return false;
    }

    if (this->wifi_client == NULL) {
        *(this->wifi_client) = this->wifi_server->available();
    }

    if (this->wifi_client->connected()) {
        return true;
    } else {
        if (this->wifi_server->status() == CLOSED) {
            int port = this->wifi_server->port();
            this->wifi_server->stop();
            this->wifi_server->begin(port);
        }

        *(this->wifi_client) = this->wifi_server->available();
        if (this->wifi_client->connected()) {
            return true;
        } else {
            return false;
        }
    }
}
