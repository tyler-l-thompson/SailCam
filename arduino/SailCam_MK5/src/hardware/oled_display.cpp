
#include <hardware/oled_display.h>

static uint32_t get_system_time_ms()
{
    return (system_get_time() / 1000);
}

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
    this->display->printf("%s\n%21s", oled_boot_message, firmware_version);
    this->display->display();
}

OledDisplay::~OledDisplay()
{
    this->display->clearDisplay();
}

void OledDisplay::dim(bool state)
{
    if (this->dim_state != state)
    {
        this->display->dim(state);
        this->dim_state = state;
    }
}

void OledDisplay::set_mode(DisplayMode mode)
{
    if (mode != this->mode) {
        switch (mode)
        {
        case DISP_DISABLED:
            this->display->setCursor(0, 0);
            this->display->clearDisplay();
            this->display->display();
            this->update_required = false;
            break;
        case DISP_ENABLED:
            this->sleep_timer = 0;
            break;
        case DISP_SLEEP_MODE:
            this->sleep_timer = get_system_time_ms();
            break;
        default:
            break;
        }
    }

    this->mode = mode;
}

void OledDisplay::update()
{
    uint32_t sys_time = get_system_time_ms();
    bool sleep_needed = ((this->mode == DISP_SLEEP_MODE) && ((sys_time - this->sleep_timer) > this->sleep_limit));

    if ((this->mode != DISP_DISABLED) && 
        this->update_required && 
        !sleep_needed)
    {
            this->display->clearDisplay();
            this->display->setCursor(0, 0);
            for (int i = 0; i < SCREEN_HEIGHT; i++) {
                this->display->println(display_buf[i]);
            }
            this->display->display();
            this->update_required = false;
            this->display_asleep = false;
    }

    if (sleep_needed & !this->display_asleep)
    {
        this->display->clearDisplay();
        this->display->setCursor(0, 0);
        this->display->display();
        this->display_asleep = true;
    }
}

void OledDisplay::wake_up()
{
    this->sleep_timer = get_system_time_ms();
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

void OledDisplay::print(const char* value)
{
    if (this->mode != DISP_DISABLED)
    {
        this->display->print(value);
        this->display->display();
    }
}

void OledDisplay::write_overview(DateTime date, IPAddress ip_address, int wifi_mode, int capture_count, int capture_mode, int capture_interval, uint8_t last_cam_error, double battery_volts)
{
    this->clear();
    snprintf(this->display_buf[0], buf_width-1, "%02d/%02d/%04d  %2d:%02d %s", date.month(), date.day(), date.year(), date.hour_12h(), date.minute(), date.am_pm());
    snprintf(this->display_buf[1], buf_width-1, "IP:%s", ip_address.toString().c_str());
    snprintf(this->display_buf[2], buf_width-1, "WM:%1u CM:%1u Bat:%1.2fv", wifi_mode, capture_mode, battery_volts);
    snprintf(this->display_buf[3], buf_width-1, "E:%1u I:%03us C:%06u", last_cam_error, capture_interval, capture_count);
    this->buf_index = buf_height;
    this->update_required = true;
}
