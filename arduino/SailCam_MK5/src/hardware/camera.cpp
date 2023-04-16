
#include <hardware/camera.h>

Camera::Camera(Storage* storage)
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

    this->storage = storage;
}

Camera::~Camera()
{
}

void Camera::process()
{
    switch(this->cur_state)
    {
        case SENSOR_POWER_DOWN:
            process_sensor_power_down();
            break;
        case SENSOR_POWER_UP:
            process_sensor_power_up();
            break;
        case CAPTURE_IMAGE:
            process_capture_image();
            break;
        case SAVE_IMAGE:
            process_save_image();
            break;
        case ERROR_HANDLE:
            process_error_handle();
            break;
        default:
            this->cur_state = SENSOR_POWER_DOWN;
            break;
    }
}

void Camera::process_sensor_power_down()
{
    if (!this->sensor_is_powered_down) /* set sensor powered down if it is needed */
    {
        this->set_sensor_power(false);
        this->sensor_is_powered_down = true;
    }
    else if (this->capture_required) /* advanced to next state when capture flag set */
    {
        this->capture_required = false;
        this->cur_state = SENSOR_POWER_UP;
    }
}

void Camera::process_sensor_power_up()
{
    this->set_sensor_power(true);
    this->sensor_is_powered_down = false;
    this->cur_state = CAPTURE_IMAGE;
}

void Camera::process_capture_image()
{
    if (!this->capture_started)
    {
        /* start capture on first call */
        this->cam->CS_LOW();
        this->cam->start_capture();
        this->cam->CS_HIGH();
        this->capture_started = true;
    }
    else
    {
        /* wait for capture to complete */
        this->cam->CS_LOW();
        uint8_t done_bit = this->cam->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK);
        this->cam->CS_HIGH();
        if (done_bit)
        {
            capture_timeout_counter = 0;
            this->capture_started = false;
            this->cur_state = SAVE_IMAGE;
        }
        else
        {
            capture_timeout_counter++;
        }

        if (capture_timeout_counter > image_capture_timeout)
        {
            capture_timeout_counter = 0;
            this->capture_started = false;
            this->cur_state = ERROR_HANDLE;
            this->last_error_reason = CAM_ERROR_CAPTURE_TIMEOUT;
        }
        delay(2);
    }
}

void Camera::process_save_image()
{
    if (!this->save_started)
    {
        /* check sd card connection */
        if (!this->storage->is_card_connected()) 
        {
            this->cur_state = ERROR_HANDLE;
            this->last_error_reason = CAM_ERROR_SD_NOT_CONNECTED;
            return;
        }

        /* create directory and file names */
        snprintf(
            this->directory_name, 
            dir_name_size, 
            "%s/%04d/%02d/%02d/%02d", 
            image_base_dir, 
            this->cur_timestamp.year(), 
            this->cur_timestamp.month(), 
            this->cur_timestamp.day(),
            this->cur_timestamp.hour()
        );
        snprintf(
            this->file_name, 
            file_name_size, 
            "%s/%04d%02d%02d_%02d%02d%02d.jpg", 
            directory_name, 
            this->cur_timestamp.year(), 
            this->cur_timestamp.month(), 
            this->cur_timestamp.day(), 
            this->cur_timestamp.hour(), 
            this->cur_timestamp.minute(), 
            this->cur_timestamp.second()
        );


        /* check and make directories */
        if (!storage->check_directory(this->directory_name))
        {
            this->cur_state = ERROR_HANDLE;
            this->last_error_reason = CAM_ERROR_DIRECTORY;
            return;
        }

        /* create new file */
        this->output_file = this->storage->open_file(this->file_name, sdfat::O_CREAT | sdfat::O_WRONLY);
        if (!this->output_file) 
        {
            this->cur_state = ERROR_HANDLE;
            this->last_error_reason = CAM_ERROR_FILE;
            return;
        }
        // this->output_file.close();

        /* store last save timestamp */
        this->last_save = this->cur_timestamp;

        /* check the length */
        this->cam->CS_LOW();
        this->length = this->cam->read_fifo_length();
        if (length >= MAX_FIFO_SIZE) // 8M - oversized
        {
            this->cur_state = ERROR_HANDLE;
            this->last_error_reason = CAM_ERROR_OVERSIZED;
            return;
        }
        if (length == 0 ) // 0kb - undersized
        {
            this->cur_state = ERROR_HANDLE;
            this->last_error_reason = CAM_ERROR_UNDERSIZED;
            return;
        }

        this->save_started = true;
        this->save_timeout_counter = 0;
    }
    else
    {
        uint8_t temp = 0, temp_last = 0;
        int transfer_size = 0;
        this->cam->CS_LOW();
        this->cam->set_fifo_burst();
        for (int i = 0; i < image_buffer_size; i++)
        {
            temp_last = temp;            
            temp = SPI.transfer(0);
            this->image_buf[i] = temp;
            transfer_size++;

            /* found the end */
            if ((temp == 0xD9) && (temp_last == 0xFF))
            {
                i = image_buffer_size;
                this->save_complete = true;
            }
        }
        this->cam->CS_HIGH();
        delay(2);
        if (!this->output_file.isOpen())
        {
            this->output_file = this->storage->open_file(this->file_name, sdfat::O_CREAT | sdfat::O_WRONLY);
            this->output_file.seek(this->seek_pos);
        }
        
        this->output_file.write(this->image_buf, transfer_size);
        this->seek_pos += transfer_size;
        // this->output_file.close();
        // this->output_file.flush();

        if (this->save_complete)
        {
            this->output_file.close();
            this->output_file.flush();
            delay(2);

            this->cam->CS_LOW();
            this->cam->clear_fifo_flag();
            this->cam->CS_HIGH();
            (this->image_count)++;
            this->seek_pos = 0;
            this->cur_state = SENSOR_POWER_DOWN;
            this->save_complete = false;
            this->save_started = false;
            this->capture_retry_counter = 0; // reset the retry counter
        }
        else 
        {
            this->save_timeout_counter++;
            if (this->save_timeout_counter > image_save_timeout)
            {
                this->cur_state = ERROR_HANDLE;
                this->last_error_reason = CAM_ERROR_SAVE_TIMEOUT;
                return;
            }
        }
        delay(2);

    }
}

void Camera::process_error_handle()
{
    this->save_complete = false;
    this->save_started = false;
    this->capture_required = true;
    this->sensor_is_powered_down = false;
    this->cur_state = SENSOR_POWER_DOWN;
    this->capture_retry_counter++;
    this->error_counter++;

    if (this->capture_retry_counter > image_capture_retries)
    {
        this->reboot_requested = true;
    }

    delay(1);
}

void Camera::capture_image(DateTime timestamp)
{
    this->cur_timestamp = timestamp;
    this->capture_required = true;
}

void Camera::set_sensor_power(bool state)
{
    uint8_t reg_read;
    this->cam->CS_LOW();
    reg_read = this->cam->read_reg(ARDUCHIP_GPIO);
    if (state == true) {
        this->cam->write_reg(ARDUCHIP_GPIO, reg_read | 0x04);
        delay(100);
        this->cam->set_format(JPEG);
        this->cam->InitCAM();
        
        this->cam->write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
        // this->cam->OV5642_set_RAW_size(OV5642_2592x1944);
        this->cam->OV5642_set_JPEG_size(OV5642_2592x1944);
        // this->cam->OV5642_set_JPEG_size(OV5642_320x240);
        // this->cam->OV5642_set_Light_Mode(Advanced_AWB);

        // Reset the CPLD
        this->cam->write_reg(0x07, 0x80);
        delay(10);
        this->cam->write_reg(0x07, 0x00);

        this->cam->set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
        this->cam->clear_fifo_flag();
        this->cam->flush_fifo();
        this->cam->write_reg(ARDUCHIP_FRAMES, 0x00);
        delay(100);
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
