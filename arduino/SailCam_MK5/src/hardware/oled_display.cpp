
#include <hardware/oled_display.h>

OledDisplay::OledDisplay()
{
    this->display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
    this->display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    this->display->display();
    this->display->setTextSize(1, 1);
    this->display->setTextColor(SSD1306_WHITE);
    this->clear();
    this->update_required = false;
    this->buf_index = 0;
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
    this->display->setCursor(0, 0);
    this->display->clearDisplay();
    this->update_required = true;
}

void OledDisplay::clear()
{
    this->display->setCursor(0, 0);
    this->display->clearDisplay();
    for (int i = 0; i < buf_height; i++) {
        for (int j = 0; j < buf_width; j++) {
            this->display_buf[i][j] = '\0';
        }
    }
    this->buf_index = 0;
    this->update_required = true;
}

char* OledDisplay::get_buffer()
{
    return *(this->display_buf);
}

void OledDisplay::write(const char* value)
{
    if (this->buf_index == buf_height) {
        for (int i = 0; i < buf_height - 1; i++) {
            memmove(this->display_buf[i], this->display_buf[i+1], buf_width);
        }
        this->buf_index--;
    }
    snprintf(this->display_buf[this->buf_index], buf_width-1, value);
    this->buf_index < buf_height ? this->buf_index++ : this->buf_index = buf_height;
    this->update_required = true;
}

void OledDisplay::writef(const char* value, ...)
{
    if (this->buf_index == buf_height) {
        for (int i = 0; i < buf_height - 1; i++) {
            memmove(this->display_buf[i], this->display_buf[i+1], buf_width);
        }
        this->buf_index--;
    }
    va_list args;
    va_start(args, value);    
    vsnprintf(this->display_buf[this->buf_index], buf_width-1, value, args);
    va_end(args);
    this->buf_index < buf_height ? this->buf_index++ : this->buf_index = buf_height;
    this->update_required = true;
}