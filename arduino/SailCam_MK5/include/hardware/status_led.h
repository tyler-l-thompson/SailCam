
#ifndef STATUS_LED_H
#define STATUS_LED_H

#include <Arduino.h>
#include <config.h>

class StatusLED
{
private:
    int get_io_pin() const { return status_led_pin; }
    int on_state, off_state;

public:
    StatusLED(int io_pin = -1, bool active_low = true);
    ~StatusLED();
    void on();
    void off();
    void blink(int count, int delay_time = 100);
    void info();
    void warn();
    void error();
};

#endif
