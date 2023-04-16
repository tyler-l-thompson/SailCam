
#include <hardware/clock.h>

Clock::Clock()
{
    rtc_clock = new DS1672();
    Wire.begin();
    rtc_clock->enable();
    delay(100); // delay seems to be needed here. otherwise time is read before clock is fully initialized.
    this->boot_time = this->get_time();
}

Clock::~Clock()
{
    //Wire.flush();
}

DateTime Clock::get_time()
{ 
    return rtc_clock->get_time(); 
}

void Clock::set_time(DateTime timestamp)
{
    rtc_clock->set_time(timestamp);
}
