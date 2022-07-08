
#include <hardware/camera.h>

Camera::Camera()
{
    pinMode(camera_chip_select, OUTPUT);
    this->cam = new ArduCAM(OV5642, camera_chip_select);
    Wire.begin();
    
    SPI.begin();
    SPI.setFrequency(8000000); // 8MHz
    
    this->cam->CS_LOW();
    this->cam->set_format(JPEG);
    this->cam->InitCAM();
    
    this->cam->write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
    // this->cam->OV5642_set_RAW_size(OV5642_2592x1944);
    this->cam->OV5642_set_JPEG_size(OV5642_2592x1944);
    // this->cam->OV5642_set_JPEG_size(OV5642_320x240);
    // this->cam->OV5642_set_Light_Mode(Advanced_AWB);

    // Reset the CPLD
    this->cam->write_reg(0x07, 0x80);
    this->cam->write_reg(0x07, 0x00);

    this->cam->set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
    this->cam->clear_fifo_flag();
    this->cam->write_reg(ARDUCHIP_FRAMES, 0x00);

    // sensor in stanby
    set_sensor_power(false);
}

Camera::~Camera()
{
}

void Camera::set_sensor_power(bool state)
{
    uint8_t reg_read;
    this->cam->CS_LOW();
    reg_read = this->cam->read_reg(ARDUCHIP_GPIO);
    if (state == true) {
        this->cam->write_reg(ARDUCHIP_GPIO, reg_read | 0x04);
        delay(250);
        this->cam->set_format(JPEG);
        this->cam->InitCAM();
        
        this->cam->write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
        // this->cam->OV5642_set_RAW_size(OV5642_2592x1944);
        this->cam->OV5642_set_JPEG_size(OV5642_2592x1944);
        // this->cam->OV5642_set_JPEG_size(OV5642_320x240);
        // this->cam->OV5642_set_Light_Mode(Advanced_AWB);

        // Reset the CPLD
        this->cam->write_reg(0x07, 0x80);
        this->cam->write_reg(0x07, 0x00);

        this->cam->set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
        this->cam->clear_fifo_flag();
        this->cam->flush_fifo();
        this->cam->write_reg(ARDUCHIP_FRAMES, 0x00);
        delay(250);
    } else {
        this->cam->write_reg(ARDUCHIP_GPIO, reg_read ^ 0x04);
    }
    this->cam->CS_HIGH();
}

bool Camera::run_self_test(char** return_message)
{
    bool self_test_pass;
    uint8_t spi_return, vid, pid, gpio_val;

    set_sensor_power(true);
    // Check if the ArduCAM SPI bus is OK
    // write 0x55 to test register, then read it back
    this->cam->CS_LOW();
    this->cam->write_reg(ARDUCHIP_TEST1, 0x55);
    spi_return = this->cam->read_reg(ARDUCHIP_TEST1);
    gpio_val = this->cam->read_reg(ARDUCHIP_GPIO);
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
    set_sensor_power(false);
    snprintf(*return_message, 47, "CAM:%s-GPIO:15|%02x,SPI:55|%02x,I2C:56|%02x:42|%02x", self_test_pass ? "True " : "False" , gpio_val, spi_return, vid, pid);
    return self_test_pass;
}

void Camera::capture_image()
{
    set_sensor_power(true);
    this->cam->CS_LOW();
    this->cam->start_capture();
    while(!this->cam->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
}

/**
 * @brief Save a captured image to the SD card.
 * 
 * @param storage Pointer to Storage driver.
 * @param file_name Name to give the file.
 * @param timestamp Timestamp of the file.
 * @return uint8_t 0 - No error, 1 - No image to save, 2 - Image oversized or buffer full, 3 - Failed to open file on SD card, 4 - SD card not connected
 */
uint8_t Camera::save_image(Storage* storage, DateTime timestamp, SerialTerminal* serial)
{
    sdfat::File32 output_file;
    uint8_t temp = 0, temp_last = 0;
    uint32_t length = 0;
    int i = 0;
    int buf_size = 1024;
    bool is_header = false;
    //char file_name[64] = {0x0};
    //char directory_name[32] = {0x0};

    //uint8_t buf[buf_size] = {0xFF};

    char* file_name = (char*) malloc(64);
    char* directory_name = (char*) malloc(32);

    uint8_t* buf = (uint8_t*) malloc(sizeof(uint8_t) * buf_size);
    for (int i = 0; i < buf_size; i++) {
        buf[i] = 0xFF;
    }

    serial->debug_println("Saving image...");
    
    if (!storage->is_card_connected()) {
        serial->debug_println("SD card not connected!");
        return 4;
    }

    // create directory and file names
    snprintf(directory_name, 32, "%s/%04d/%02d/%02d", image_base_dir, timestamp.year(), timestamp.month(), timestamp.day());
    snprintf(file_name, 64, "%s/%04d%02d%02d_%02d%02d%02d.jpg", directory_name, timestamp.year(), timestamp.month(), timestamp.day(), timestamp.hour(), timestamp.minute(), timestamp.second());

    serial->debug_printf("Saving image to: %s\r\n", file_name);

    // check and make directories
    storage->check_directory(directory_name);

    this->last_save = timestamp;

    this->cam->CS_LOW();

    // check the length
    length = this->cam->read_fifo_length();
    serial->debug_printf("Size: %d Byes\r\n", length);
    if (length >= MAX_FIFO_SIZE) // 8M - oversized
    {
        serial->debug_println("Oversized!");
        free(file_name);
        free(directory_name);
        free(buf);
        return 2;
    }
    if (length == 0 ) // 0kb - undersized
    {
        serial->debug_println("Undersized!");
        free(file_name);
        free(directory_name);
        free(buf);
        return 1;
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
            serial->debug_println("Done.");
            buf[i++] = temp;  // save the last  0XD9     
            // Write the remain bytes in the buffer
            this->cam->CS_HIGH();
            output_file.write(buf, i);

            // Close the file
            output_file.close();
            serial->debug_println("File closed.");
            is_header = false;
            this->cam->CS_LOW();
            this->cam->set_fifo_burst();
            i = 0;
        }  

        if (is_header == true)
        { 
            // Write image data to buffer if not full
            if (i < buf_size)
            {
                buf[i++] = temp;
            }
            else
            {
                // Write image_buffer_size bytes image data to file
                this->cam->CS_HIGH();
                output_file.write(buf, buf_size);
                serial->debug_print(".");
                delay(10);
                i = 0;
                buf[i++] = temp;
                this->cam->CS_LOW();
                this->cam->set_fifo_burst();
            }        
        }
        else if ((temp == 0xD8) & (temp_last == 0xFF))
        {
            is_header = true;
            this->cam->CS_HIGH();

            serial->debug_println("Opening file...");
            // output_file.open(file_name, sdfat::O_WRITE);
            output_file = storage->open_file(file_name, sdfat::O_CREAT | sdfat::O_WRONLY);
            delay(100);
            // output_file = (storage->open_file(file_name, FILE_WRITE));

            if (!output_file)
            {
                set_sensor_power(false);
                serial->debug_println("Failed to open file!");
                free(file_name);
                free(directory_name);
                free(buf);
                return 3;
            }
            serial->debug_println("File opened successfully.");

            this->cam->CS_LOW();
            this->cam->set_fifo_burst();   
            buf[i++] = temp_last;
            buf[i++] = temp;   
            serial->debug_print("Beginning transfer...");
        }
    }
    this->cam->clear_fifo_flag();
    set_sensor_power(false);
    (this->image_count)++;
    serial->debug_println("Image save complete.");
    free(file_name);
    free(directory_name);
    free(buf);
    return 0;
}
