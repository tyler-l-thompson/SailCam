
#include <hardware/camera.h>

Camera::Camera()
{
    pinMode(camera_chip_select, OUTPUT);
    this->cam = new ArduCAM(OV5642, camera_chip_select);
    Wire.begin();
    
    SPI.begin();
    SPI.setFrequency(8000000); // 8MHz
    
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

bool Camera::run_self_test(char** return_message)
{
    bool self_test_pass;
    uint8_t spi_return, vid, pid;

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

    if (spi_return == 0x55 && vid == 0x56 && pid == 0x42)
    {
        self_test_pass = true;
    } else
    {
        self_test_pass = false;
    }

    snprintf(*return_message, 36, "CAM:%s-SPI:55|%02x,I2C:56|%02x:42|%02x", self_test_pass ? "True " : "False" , spi_return, vid, pid);
    return self_test_pass;
}

void Camera::capture_image()
{
    this->cam->start_capture();
    while(!this->cam->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
}

bool Camera::save_image(Storage* storage, char* file_name)
{
    File output_file;
    uint8_t temp = 0, temp_last = 0;
    uint32_t length = 0;
    static int i = 0;
    
    byte buf[256]; 
    
    // check the length
    length = this->cam->read_fifo_length();
    if (length >= MAX_FIFO_SIZE) // 8M - oversized
    {
        return false;
    }
    if (length == 0 ) // 0kb - undersized
    {
        return false;
    } 

    this->cam->CS_LOW();
    this->cam->set_fifo_burst(); // Set fifo burst mode
    i = 0;
    while (length--)
    {
        temp_last = temp;
        temp = SPI.transfer(0x00);

        // Read JPEG data from FIFO
        if ( (temp == 0xD9) && (temp_last == 0xFF) ) // If find the end ,break while,
        {
            buf[i++] = temp;  // save the last  0XD9     
            // Write the remain bytes in the buffer
            this->cam->CS_HIGH();
            output_file.write(buf, i);

            // Close the file
            output_file.close();

            this->is_header = false;
            this->cam->CS_LOW();
            this->cam->set_fifo_burst();
            i = 0;
        }  

        if (this->is_header)
        { 
            // Write image data to buffer if not full
            if (i < 256)
            {
                buf[i++] = temp;
            }
            else
            {
                // Write 256 bytes image data to file
                this->cam->CS_HIGH();
                output_file.write(buf, 256);
                i = 0;
                buf[i++] = temp;
                this->cam->CS_LOW();
                this->cam->set_fifo_burst();
            }        
        }
        else if ((temp == 0xD8) & (temp_last == 0xFF))
        {
            this->is_header = true;
            this->cam->CS_HIGH();

            // output_file = SD.open(filename, FILE_WRITE);
            output_file = storage->open_file(file_name, FILE_WRITE);
            delay(100);
            // if (!output_file)
            // {
            //     Serial.println(F("File open failed"));
            //     while (1) {
            //     blink_led(100, 3);
            //     delay(1000);
            //     }
            // }
            this->cam->CS_LOW();
            this->cam->set_fifo_burst();   
            buf[i++] = temp_last;
            buf[i++] = temp;   
        }
    }

    this->cam->CS_HIGH();
    return true;
}