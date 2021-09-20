
#include <hardware/camera.h>

Camera::Camera()
{
    Wire.begin();
    
    
    pinMode(camera_chip_select, OUTPUT);
    digitalWrite(camera_chip_select, HIGH);
    this->cam = new ArduCAM(OV5642, camera_chip_select);
    //this->cam->CS_LOW();
    
    SPI.begin();
    //SPI.setFrequency(4000000); //4MHz
    
    //Reset the CPLD
    this->cam->write_reg(0x07, 0x80);
    delay(100);
    this->cam->write_reg(0x07, 0x00);
    delay(100); 

    // this->cam->set_format(JPEG);
    // this->cam->InitCAM();
    // this->cam->set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
    // this->cam->clear_fifo_flag();
    // this->cam->write_reg(ARDUCHIP_FRAMES, 0x00);
    // this->cam->OV5642_set_JPEG_size(OV5642_2592x1944);
}

Camera::~Camera()
{
}

void Camera::init_cam()
{
    //this->cam->CS_LOW();
    this->cam->set_format(JPEG);
    this->cam->InitCAM();
    this->cam->set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
    this->cam->clear_fifo_flag();
    this->cam->write_reg(ARDUCHIP_FRAMES, 0x00);
    this->cam->OV5642_set_JPEG_size(OV5642_2592x1944);
    //this->cam->CS_HIGH();
}

const char* Camera::run_self_test()
{
    //Check if the ArduCAM SPI bus is OK
    //this->cam->CS_LOW();
    this->cam->write_reg(ARDUCHIP_TEST1, 0x55);
    uint8_t temp = this->cam->read_reg(ARDUCHIP_TEST1);
    //this->cam->CS_HIGH();
    if (temp != 0x55){
        const char* return_str = "Cam: SPI Error 0000";
        snprintf((char*)return_str, 19, "Cam: SPI Error %d", temp);
        return return_str;
    }

    //Check if the camera module type is OV5642
    uint8_t vid, pid;
    this->cam->wrSensorReg16_8(0xff, 0x01);
    this->cam->rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
    this->cam->rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
    if((vid != 0x42) || (pid != 0x42)){
        const char* return_str = "Cam: I2C Error 0000 0000";
        snprintf((char*)return_str, 24, "Cam: I2C Error %d %d", vid, pid);
        return return_str;
    }

    return "Cam: Connected";
}