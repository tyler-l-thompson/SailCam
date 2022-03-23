
#include <hardware/camera.h>

Camera::Camera()
{
    pinMode(camera_chip_select, OUTPUT);
    this->cam = new ArduCAM(OV5642, camera_chip_select);
    Wire.begin();
    
    SPI.begin();
    SPI.setFrequency(4000000); // 4MHz
    
    this->cam->set_format(JPEG);
    this->cam->InitCAM();
    this->cam->write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
    this->cam->OV5642_set_JPEG_size(OV5642_2592x1944);

    // Reset the CPLD
    this->cam->write_reg(0x07, 0x80);
    this->cam->write_reg(0x07, 0x00);

    this->cam->set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
    this->cam->clear_fifo_flag();
    this->cam->write_reg(ARDUCHIP_FRAMES, 0x00);

}

Camera::~Camera()
{
}

const char* Camera::run_self_test()
{
    uint8_t spi_return, vid, pid;
    const char * return_str = new char[41];

    // Check if the ArduCAM SPI bus is OK
    // write 0x55 to test register, then read it back
    this->cam->CS_LOW();
    this->cam->write_reg(ARDUCHIP_TEST1, 0x55);
    spi_return = this->cam->read_reg(ARDUCHIP_TEST1);
    this->cam->CS_HIGH();

    // Check if the camera module type is OV5642
    // read chip ID registers
    this->cam->wrSensorReg16_8(0xff, 0x01);
    this->cam->rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
    this->cam->rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);

    snprintf((char*) return_str, 41, "CAM - SPI 55:%02x, I2C 56:%02x 42:%02x", spi_return, vid, pid);
    return return_str;
}