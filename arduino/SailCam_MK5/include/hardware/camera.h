
#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <config.h>
#include <Wire.h>
#include <SPI.h>
#include "memorysaver.h"
#include <ArduCAM.h>

class Camera
{
private:
    //ArduCAM* cam;

public:
    Camera();
    ~Camera();
    const char* run_self_test();
    ArduCAM* cam;
};

#endif
