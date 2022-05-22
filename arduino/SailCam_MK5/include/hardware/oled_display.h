
#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <config.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS1672.h>
#include <ESP8266WiFi.h>

#define SCREEN_ADDRESS 0x3D
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define buf_width 25
#define buf_height 4

class OledDisplay
{
private:
    Adafruit_SSD1306* display;
    char display_buf[buf_height][buf_width];
    int buf_index;
    bool update_required;

public:
    OledDisplay();
    ~OledDisplay();
    void update();
    void disable();
    void clear();
    char* get_buffer();
    void write(const char* value);
    void writef(const char* value, ...);
    void write_overview(DateTime date, IPAddress ip_address, char* wifi_ssid);
};

#endif
