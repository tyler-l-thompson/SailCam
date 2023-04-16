
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
    char log_folder_path[11 + log_folder_base_dir_length];
    char log_file_name[21];
    char log_file_path[32 + log_folder_base_dir_length];
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
    bool log_data_point(DateTime timestamp, char* data, bool more_to_log = false);
    void log_error(const char* data);
    void log_error_code(int code);
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
    int list_directory(sdfat::File32 dir, char** buffer, uint32_t buffer_length, uint32_t start_pos, const char* prefix, const char* suffix, bool links = false, const char* link_root = NULL);
    uint8_t get_sd_error();
    bool file_exists(const char* file_path);
    void rm_file(char* file_path);
    void rm_dir(char* dir_path);
    void increment_counter(char * key, bool write_back);
    char* get_log_folder_path() { return this->log_folder_path; };
    char* get_log_file_name() { return this->log_file_name; };
    char* get_log_path() { return this->log_file_path; };
    bool is_directory(char* dir_path);
};

void sd_datetime_callback(uint16_t* date, uint16_t* time);



#endif
