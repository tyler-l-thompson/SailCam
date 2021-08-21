
#include <hardware/status_led.h>

StatusLED::StatusLED(int io_pin, bool active_low)
{
    int pin = (io_pin == -1) ? get_io_pin() : io_pin;
    pinMode(pin, OUTPUT);

    if (active_low) {
        on_state = LOW;
        off_state = HIGH;
    } else {
        on_state = HIGH;
        off_state = LOW;
    }

    off();
}

StatusLED::~StatusLED()
{
}

void StatusLED::on()
{
    digitalWrite(get_io_pin(), on_state);
}

void StatusLED::off()
{
    digitalWrite(get_io_pin(), off_state);
}

void StatusLED::blink(int count, int delay_time)
{
    for (int i = 0; i < count; i++) {
        on();
        delay(delay_time);
        off();
        delay(delay_time);
    }
}

void StatusLED::info()
{
    blink(1, 100);
}

void StatusLED::warn()
{
    blink(2, 50);
}

void StatusLED::error()
{
    blink(3, 50);
}
