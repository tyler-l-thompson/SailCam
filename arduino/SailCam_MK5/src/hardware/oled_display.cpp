
#include <hardware/oled_display.h>

OledDisplay::OledDisplay()
{
    this->display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
    this->display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    //this->display->display();
    this->display->setTextSize(1);
    this->display->setTextColor(SSD1306_WHITE);
    this->display->clearDisplay();
    this->update_required = false;
}

OledDisplay::~OledDisplay()
{
    this->display->clearDisplay();
}

void OledDisplay::update()
{
    if (this->update_required) {
        this->display->clearDisplay();
        this->display->setCursor(0, 0);
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            this->display->println(display_buf[i]);
        }
        this->display->display();
        this->update_required = false;
    }
}

void OledDisplay::disable()
{
    this->display->setCursor(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->display->clearDisplay();
    this->update_required = true;
}

void OledDisplay::clear()
{
    this->display->setCursor(0, 0);
    this->display->clearDisplay();
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            this->display_buf[i][j] = '\0';
        }
    }
    this->update_required = true;
}

char* OledDisplay::get_buffer()
{
    return *(this->display_buf);
}

void OledDisplay::write(const char* value)
{
    snprintf(this->display_buf[0], SCREEN_WIDTH-1, value);
    this->update_required = true;
    // this->display->clearDisplay();
    // this->display->println(value);
    // this->display->display();
}
