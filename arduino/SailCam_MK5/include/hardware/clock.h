
#ifndef CLOCK_H
#define CLOCK_H

#include <DS1672.h>
#include <Wire.h>

class Clock
{
private:
    DS1672* rtc_clock;
    DateTime boot_time;

public:
    Clock();
    ~Clock();
    DateTime get_time();
    void set_time(DateTime timestamp);
    DateTime get_boot_time() { return this->boot_time; };
};

#endif
