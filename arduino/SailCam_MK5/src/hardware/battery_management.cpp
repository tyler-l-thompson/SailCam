#include <hardware/battery_management.h>

BatteryManagement::BatteryManagement()
{
    pinMode(battery_measure_op_disable_pin, OUTPUT);
    disable_op_amp();
}

BatteryManagement::~BatteryManagement()
{

}

void BatteryManagement::enable_op_amp()
{
    digitalWrite(battery_measure_op_disable_pin, HIGH);
}

void BatteryManagement::disable_op_amp()
{
    digitalWrite(battery_measure_op_disable_pin, LOW);
}

double BatteryManagement::get_battery_volts()
{
    enable_op_amp();
    delay(10);
    int amp_enabled_measure = analogRead(battery_measure_adc_channel);
    disable_op_amp();
    return (battery_max_volts * amp_enabled_measure) / battery_measure_adc_resolution;
}

double BatteryManagement::get_system_volts()
{
    disable_op_amp();
    delay(10);
    return (battery_max_volts * analogRead(battery_measure_adc_channel)) / battery_measure_adc_resolution;
}
