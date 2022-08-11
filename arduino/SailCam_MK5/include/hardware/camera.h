
#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <config.h>
#include <Wire.h>
#include <SPI.h>
#include <hardware/storage.h>
#include "memorysaver.h"
#include <ArduCAM.h>

#define image_buffer_size 4096
#define image_capture_timeout 500
#define image_capture_retries 2

class Camera
{
private:
    //ArduCAM* cam;
    
    DateTime last_save;
    int image_count = 0;

public:
    Camera();
    ~Camera();
    ArduCAM* cam;
    void set_sensor_power(bool state);
    bool run_self_test(char** return_message);
    int capture_image(SerialTerminal* serial);
    uint8_t save_image(Storage* storage, DateTime timestmap, SerialTerminal* serial);
    DateTime get_last_save() {return this->last_save;};
    int get_image_count() {return this->image_count;};
};

#endif
