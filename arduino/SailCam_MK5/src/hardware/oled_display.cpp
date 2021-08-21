
#include <hardware/oled_display.h>

#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

OledDisplay::OledDisplay()
{
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.display();
    delay(2000);
    display.clearDisplay();
    display.drawPixel(10, 10, SSD1306_WHITE);
    display.display();
}

OledDisplay::~OledDisplay()
{
    
}
