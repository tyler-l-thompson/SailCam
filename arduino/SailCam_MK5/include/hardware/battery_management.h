#ifndef BATTERY_MANAGEMENT_H
#define BATTERY_MANAGEMENT_H

#include <Arduino.h>
#include <config.h>

class BatteryManagement
{
private:
    void enable_op_amp();
    void disable_op_amp();

public:
    BatteryManagement();
    ~BatteryManagement();
    double get_battery_volts();
    double get_system_volts();
    int get_battery_adc();
};

#endif