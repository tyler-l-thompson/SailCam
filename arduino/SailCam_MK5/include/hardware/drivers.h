
#ifndef DRIVERS_H
#define DRIVERS_H

#include <hardware/clock.h>
#include <hardware/storage.h>
#include <hardware/status_led.h>
#include <hardware/serial_terminal.h>
#include <hardware/wifi.h>
#include <hardware/battery_management.h>
#include <hardware/oled_display.h>
#include <hardware/camera.h>
#include "user_interface.h"

// a struct to hold pointers to all hardware classes
struct HardwareDrivers {
    Clock* system_clock;
    Storage* storage_controller;
    SerialTerminal* serial_term;
    StatusLED* status_led;
    Wifi* wifi_radio;
    BatteryManagement* battery_management;
    OledDisplay* old_display;
    Camera* camera;
};

static inline uint32_t get_safe_buffer_size(uint32_t requested_sized)
{
    uint32_t free_heap = system_get_free_heap_size();
    uint32_t safe_size = requested_sized;

    if ((free_heap - requested_sized) < min_heap_size)
    {
        if ((free_heap - min_heap_size) < 0)
        {
            safe_size = 0;
        }
        else
        {
            safe_size = free_heap - min_heap_size;
        }
    }

    return safe_size;
}

#endif
