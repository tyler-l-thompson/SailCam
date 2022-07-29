
#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdlib.h>
#include <hardware/drivers.h>

#define commands_size 24

class CommandParser
{

typedef void (CommandParser::*command_function)(char* arg, char* param, char** message);

struct CommandStruct {
    const char* name;
    command_function function;
};

private:
    // command functions
    void restart(char* arg, char* param, char** message);
    void get_time(char* arg, char* param, char** message);
    void set_time(char* arg, char* param, char** message);
    void get_var(char* arg, char* param, char** message);
    void set_var(char* arg, char* param, char** message);
    void get_config(char* arg, char* param, char** message);
    void save_config(char* arg, char* param, char** message);
    void get_free_memory(char* arg, char* param, char** message);
    void get_cpu_speed(char* arg, char* param, char** message);
    void get_battery_level(char* arg, char* param, char** message);
    void get_wifi_status(char* arg, char* param, char** message);
    void get_firmware_version(char* arg, char* param, char** message);
    void blink_led(char* arg, char* param, char** message);
    void check_sd_card(char* arg, char* param, char** message);
    void format_sd_card(char* arg, char* param, char** message);
    void check_camera(char* arg, char* param, char** message);
    void capture_image(char* arg, char* param, char** message);
    void write_spi_reg(char* arg, char* param, char** message);
    void read_spi_reg(char* arg, char* param, char** message);
    void write_i2c_reg(char* arg, char* param, char** message);
    void read_i2c_reg(char* arg, char* param, char** message);
    void list_directory(char* arg, char* param, char** message);
    void make_directory(char* arg, char* param, char** message);
    void save_config_defaults(char* arg, char* param, char** message);

    HardwareDrivers* hardware_drivers;
    CommandStruct commands[commands_size];

public:
    CommandParser(HardwareDrivers* hardware_drivers);
    ~CommandParser();
    bool process_serial_terminal();
    void process_web_api(const char* command_name, const char* arg, const char* param, char** message);
    void process_tcp_api();

};

#endif
