
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
#define buf_width 22 // display actual width is 21 characters
#define buf_height 4
#define dim_begin_hour 21
#define dim_end_hour 5

enum DisplayMode { DISP_DISABLED, DISP_ENABLED, DISP_SLEEP_MODE };

class OledDisplay
{
private:
    Adafruit_SSD1306* display;
    char display_buf[buf_height][buf_width];
    int buf_index;
    bool update_required;
    DisplayMode mode = DISP_ENABLED;
    uint32_t sleep_timer = 0;
    uint32_t sleep_limit = 0;
    bool display_asleep = false;
    bool dim_state = false;

public:
    OledDisplay();
    ~OledDisplay();
    void dim(bool state);
    void set_sleep_time(int sleep_time) { this->sleep_limit = sleep_time; };
    void set_mode(DisplayMode mode);
    void update();
    void wake_up();
    void clear();
    char* get_buffer();
    void write(const char* value);
    void writef(const char* value, ...);
    void print(const char* value);
    void write_overview(DateTime date, IPAddress ip_address, int wifi_mode, int capture_count, int capture_mode, int capture_interval, uint8_t last_cam_error, double battery_volts);

};

#endif
