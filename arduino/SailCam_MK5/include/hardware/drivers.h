
#ifndef DRIVERS_H
#define DRIVERS_H

#include <hardware/clock.h>
#include <hardware/storage.h>
#include <hardware/status_led.h>
#include <hardware/serial_terminal.h>
#include <hardware/wifi.h>
#include <hardware/battery_management.h>

struct HardwareDrivers {
    Clock* system_clock;
    Storage* storage_controller;
    SerialTerminal* serial_term;
    StatusLED* status_led;
    Wifi* wifi_radio;
    BatteryManagement* battery_management;
};

#endif
