
#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <config.h>
#include <Wire.h>
#include <SPI.h>
#include <hardware/storage.h>
#include "memorysaver.h"
#include <ArduCAM.h>

class Camera
{
private:
    //ArduCAM* cam;
    bool is_header;
    DateTime last_save;

public:
    Camera();
    ~Camera();
    ArduCAM* cam;
    void set_sensor_power(bool state);
    bool run_self_test(char** return_message);
    void capture_image();
    bool save_image(Storage* storage, char* file_name, DateTime timestmap);
    DateTime get_last_save();
};

#endif
