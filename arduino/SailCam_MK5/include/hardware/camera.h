
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
#define image_capture_timeout 1000
#define image_capture_retries 2
#define dir_name_size 35
#define file_name_size 64
#define image_save_timeout 1000
#define camera_settings_length 9

#define camera_setting_out_of_range 13

enum CameraState {
    SENSOR_POWER_DOWN, 
    SENSOR_POWER_UP, 
    CAPTURE_IMAGE, 
    SAVE_IMAGE, 
    ERROR_HANDLE
};

enum CameraErrorReason { 
    CAM_ERROR_UNKNOWN, 
    CAM_ERROR_CAPTURE_TIMEOUT, 
    CAM_ERROR_SD_NOT_CONNECTED, 
    CAM_ERROR_DIRECTORY, 
    CAM_ERROR_FILE, 
    CAM_ERROR_OVERSIZED, 
    CAM_ERROR_UNDERSIZED, 
    CAM_ERROR_SAVE_TIMEOUT 
};

enum CameraSettingsMode{
    CAM_SETTINGS_MODE_DEFAULT,
    CAM_SETTINGS_MODE_USER
};

typedef void (ArduCAM::*camera_setting_function)(uint8_t value);

struct CameraSetting_T{
    const char* name;
    camera_setting_function function_p;
    uint8_t value;
};

class Camera
{
private:
    //ArduCAM* cam;
    DateTime cur_timestamp;
    DateTime last_save;
    Storage * storage;
    sdfat::File32 output_file;
    char directory_name[dir_name_size];
    char file_name[file_name_size];
    uint8_t image_buf[image_buffer_size];
    int image_count = 0;
    uint32_t length = 0;
    uint32_t seek_pos = 0;

    CameraState cur_state = SENSOR_POWER_DOWN;
    bool capture_required = false;
    bool sensor_is_powered_down = false;
    bool capture_started = false;
    bool save_started = false;
    bool save_complete = false;

    uint32_t capture_timeout_counter = 0;
    uint8_t capture_retry_counter = 0;
    uint32_t save_timeout_counter = 0;
    uint32_t error_counter = 0;

    CameraErrorReason last_error_reason = CAM_ERROR_UNKNOWN;

    bool reboot_requested = false;

    CameraSetting_T camera_settings[camera_settings_length];

    void process_sensor_power_down();
    void process_sensor_power_up();
    void process_capture_image();
    void process_save_image();
    void process_error_handle();

    

public:
    Camera(Storage * storage);
    ~Camera();
    ArduCAM* cam;
    void process();
    bool run_self_test(char** return_message);
    void capture_image(DateTime timestamp);
    bool is_capture_done() { return (this->cur_state == SENSOR_POWER_DOWN); };
    DateTime get_last_save() {return this->last_save;};
    int get_image_count() {return this->image_count;};
    CameraState get_state() { return this->cur_state; };
    char * get_last_save_file_name() { return this->file_name; };
    void set_sensor_power(bool state);
    uint32_t get_error_counter() { return this->error_counter; };
    CameraErrorReason get_error_reason() { return this->last_error_reason; };
    bool is_reboot_requested() { return this->reboot_requested; };
    bool set_camera_setting(const char* settings, uint8_t value);
    CameraSetting_T* get_camera_settings() { return this->camera_settings; };
    bool write_camera_settings();
    bool read_camera_settings();
    const char* get_camera_setting_value_description(uint8_t camera_setting_idx);
};

#endif
