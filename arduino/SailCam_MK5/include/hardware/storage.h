
#ifndef STORAGE_H
#define STORAGE_H

#include <SPI.h>
#include <SD.h>
#include <hardware/serial_terminal.h>
#include <hardware/clock.h>
#include <settings/system_configuration.h>
#include <config.h>
#include <stddef.h>
#include <stdlib.h>


class Storage
{
private:
    bool card_connected;
    char log_folder_path[14 + log_folder_base_dir_length];
    char log_file_name[29];
    char log_file_path[38 + log_folder_base_dir_length];
    char formatted_timestamp[25];
    
    void set_log_paths(DateTime timestamp);
    void set_formatted_timestamp(DateTime timestamp);
    void write_settings_defaults(File settings_file, int length);
    SystemConfiguration* system_configuration;
    File* log_file;

public:
    Storage(DateTime* timestamp);
    ~Storage();
    char* get_formatted_timestamp();
    bool check_and_reconnect_card();
    bool is_card_connected();
    bool log_data_point(DateTime timestamp, char* data);
    void check_directory(char* dir_name);
    SystemConfiguration* get_system_configuration();
    File open_file(char* file_path, uint8_t mode = FILE_READ);
    File open_file(const char* file_path, uint8_t mode = FILE_READ);
    int get_file_line_count(File working_file);
    void read_settings_file(SerialTerminal* serial_term);
    void print_configuration(char** message_buf);
    void print_configuration(SerialTerminal* serial_term);
    void write_settings_file(char** message_buf);
};

void sd_datetime_callback(uint16_t* date, uint16_t* time);



#endif
