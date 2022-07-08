
#ifndef CLOCK_H
#define CLOCK_H

#include <DS1672.h>
#include <Wire.h>

class Clock
{
private:
    DS1672* rtc_clock;

public:
    Clock();
    ~Clock();
    DateTime get_time();
    void set_time(DateTime timestamp);
    
};

#endif
