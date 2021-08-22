
#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <config.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

class OledDisplay
{
private:
    Adafruit_SSD1306* display;
    char display_buf[SCREEN_HEIGHT][SCREEN_WIDTH];
    bool update_required;

public:
    OledDisplay();
    ~OledDisplay();
    void update();
    void disable();
    void clear();
    char* get_buffer();
    void write(const char* value);
};

#endif
