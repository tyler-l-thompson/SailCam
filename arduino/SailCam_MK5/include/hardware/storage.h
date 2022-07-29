
#ifndef STORAGE_H
#define STORAGE_H

#include <SPI.h>
#include <SdFat.h>
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
    
    SystemConfiguration* system_configuration;
    sdfat::File32* log_file;
    sdfat::SdFat32 SD;
    

public:
    Storage(DateTime* timestamp);
    ~Storage();
    char* get_formatted_timestamp();
    bool check_and_reconnect_card();
    bool is_card_connected();
    bool log_data_point(DateTime timestamp, char* data);
    bool directory_exists(char* dir_name);
    bool check_directory(char* dir_name);
    SystemConfiguration* get_system_configuration();
    sdfat::File32 open_file(char* file_path, uint8_t mode = sdfat::O_READ);
    sdfat::File32 open_file(const char* file_path, uint8_t mode = sdfat::O_READ);
    bool format_sd_card(sdfat::print_t* pr = nullptr);
    int get_file_line_count(sdfat::File32 working_file);
    void read_settings_file(SerialTerminal* serial_term);
    void print_configuration(char** message_buf);
    void print_configuration(SerialTerminal* serial_term);
    int write_settings_file(char** message_buf);
    int write_settings_file();
    void write_settings_defaults();
    uint8_t get_sd_card_type();
    int get_free_space();
    int list_directory(sdfat::File32 dir, char** buffer, int buffer_length, int start_pos, const char* prefix, const char* suffix);
    uint8_t get_sd_error();
};

void sd_datetime_callback(uint16_t* date, uint16_t* time);



#endif
