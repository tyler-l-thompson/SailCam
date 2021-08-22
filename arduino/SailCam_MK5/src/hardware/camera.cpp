
#include <hardware/camera.h>

Camera::Camera()
{
    pinMode(camera_chip_select, OUTPUT);
    digitalWrite(camera_chip_select, HIGH);
}

Camera::~Camera()
{
    
}
