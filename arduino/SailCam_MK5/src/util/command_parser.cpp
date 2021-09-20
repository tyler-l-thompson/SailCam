
#include <util/command_parser.h>

CommandParser::CommandParser(HardwareDrivers* hardware_drivers)
: commands {
                {"restart", &CommandParser::restart},
                {"gettime", &CommandParser::get_time},
                {"settime", &CommandParser::set_time},
                {"getvar", &CommandParser::get_var},
                {"setvar", &CommandParser::set_var},
                {"getconfig", &CommandParser::get_config},
                {"saveconfig", &CommandParser::save_config},
                {"getmem", &CommandParser::get_free_memory},
                {"getbat", &CommandParser::get_battery_level},
                {"getwifi", &CommandParser::get_wifi_status},
                {"getfirmware", &CommandParser::get_firmware_version},
                {"blinkled", &CommandParser::blink_led},
                {"checksdcard", &CommandParser::check_sd_card},
                {"formatsd", &CommandParser::format_sd_card},
                {"checkcam", &CommandParser::check_camera}
            }
{
    this->hardware_drivers = hardware_drivers;
}

CommandParser::~CommandParser()
{
}

/**
 * Command functions that can be called from the API
 **/

void CommandParser::restart(char* arg, char* param, char** message) 
{
    ESP.restart();
}

void CommandParser::get_time(char* arg, char* param, char** message)
{
    DateTime timestamp = hardware_drivers->system_clock->get_time();
    snprintf(*message, 25, "%02d/%02d/%04d %02d:%02d:%02d", timestamp.month(), timestamp.day(), timestamp.year(), timestamp.hour(), timestamp.minute(), timestamp.second());
}

void CommandParser::set_time(char* arg, char* param, char** message)
{
    DateTime new_timestamp;
    char year[5], month[3], day[3], hour[3], minute[3], second[3];
    int timestamp_str_length;

    // check that the timestamp is 14 characters long
    for (timestamp_str_length = 0; arg[timestamp_str_length] != '\0'; timestamp_str_length++);
    if (timestamp_str_length != 14) {
        sprintf(*message, "Bad time stamp format. YYYYMMDDhhmmss");
        return;
    }

    strncpy(year, arg, 4);
    year[4] = '\0';
    strncpy(month, &(arg[4]), 2);
    month[2] = '\0';
    strncpy(day, &(arg[6]), 2);
    day[2] = '\0';
    strncpy(hour, &(arg[8]), 2);
    hour[2] = '\0';
    strncpy(minute, &(arg[10]), 2);
    minute[2] = '\0';
    strncpy(second, &(arg[12]), 2);
    second[2] = '\0';

    new_timestamp = DateTime(atoi(year), atoi(month), atoi(day), atoi(hour), atoi(minute), atoi(second));
    hardware_drivers->system_clock->set_time(new_timestamp);

    DateTime timestamp = hardware_drivers->system_clock->get_time();
    sprintf(*message, "%02d/%02d/%04d %02d:%02d:%02d", timestamp.month(), timestamp.day(), timestamp.year(), timestamp.hour(), timestamp.minute(), timestamp.second());

}

void CommandParser::get_var(char* arg, char* param, char** message)
{
    if (hardware_drivers->storage_controller->get_system_configuration()->verify_setting_key(arg)) {
        sprintf(*message, "%s=%s", arg, hardware_drivers->storage_controller->get_system_configuration()->get_setting(arg)->get_value_str());
    } else {
        sprintf(*message, "System setting not found: %s", arg);
    }
}

void CommandParser::set_var(char* arg, char* param, char** message)
{
    char* new_value;
    int new_size;
    if (hardware_drivers->storage_controller->get_system_configuration()->verify_setting_key(arg)) {
        new_size = strlen(param);
        new_value = (char*) malloc(new_size);
        snprintf(new_value, new_size+1, param);
        hardware_drivers->storage_controller->get_system_configuration()->update_setting(arg, new_value, new_size);
        //free(new_value);
        snprintf(*message, command_message_buffer_length, "%s=%s", arg, hardware_drivers->storage_controller->get_system_configuration()->get_setting(arg)->get_value_str());
    } else {
        snprintf(*message, command_message_buffer_length, "System setting not found: %s", arg);
    }
}

void CommandParser::get_config(char* arg, char* param, char** message)
{
    hardware_drivers->storage_controller->print_configuration(message);
}

void CommandParser::save_config(char* arg, char* param, char** message)
{
    hardware_drivers->storage_controller->write_settings_file(message);
}

void CommandParser::get_free_memory(char* arg, char* param, char** message)
{
    sprintf(*message, "free_memory=%d, units=Bytes", system_get_free_heap_size());
}

void CommandParser::get_battery_level(char* arg, char* param, char** message)
{
    double battery_volts, system_volts, battery_percent;
    int battery_adc;
    battery_volts = hardware_drivers->battery_management->get_battery_volts();
    system_volts = hardware_drivers->battery_management->get_system_volts();
    battery_adc = hardware_drivers->battery_management->get_battery_adc();
    battery_percent = ((battery_volts - battery_min_volts) * 100 / (battery_max_volts - battery_min_volts));
    sprintf
    (
        *message, 
        "battery_percent=%0.2f%%, battery_volts=%0.2fv, battery_adc=%d vcc_volts=%0.2fv", 
        battery_percent, 
        battery_volts, 
        battery_adc,
        system_volts
    );
}

void CommandParser::get_wifi_status(char* arg, char* param, char** message)
{
    bool client_status = hardware_drivers->wifi_radio->is_client_connected();
    sprintf
    (
        *message, 
        "wifi_status=%s, ip_address=%s, mac_address=%s, client_connected=%s", 
        hardware_drivers->wifi_radio->get_connection_status_str(), 
        hardware_drivers->wifi_radio->get_ip_address().toString().c_str(),
        hardware_drivers->wifi_radio->get_mac_address(),
        client_status ? "true" : "false"
    );
}

void CommandParser::get_firmware_version(char* arg, char* param, char** message)
{
    sprintf(*message, "firmware_version=%s", firmware_version);
}

void CommandParser::blink_led(char* arg, char* param, char** message)
{
    int blink_count = (strlen(arg) == 0) ? 4 : atoi(arg);
    int blink_delay = (strlen(param) == 0) ? 100 : atoi(param);
    sprintf(*message, "count=%d, delay=%dms", blink_count, blink_delay);
    hardware_drivers->status_led->blink(blink_count, blink_delay);
}

void CommandParser::check_sd_card(char* arg, char* param, char** message)
{
    uint8_t sd_type = 0;
    int timeout = 10;
    while(hardware_drivers->storage_controller->check_and_reconnect_card() != true && timeout > 0) {
        delay(100);
        timeout--;
    }
    if (timeout < 0) {
        sd_type = hardware_drivers->storage_controller->get_sd_card_type();
    }
    sprintf(*message, "sd_card_connected=%s, sd_card_type=%d", hardware_drivers->storage_controller->check_and_reconnect_card() ? "True" : "False", sd_type);
}

void CommandParser::format_sd_card(char* arg, char* param, char** message)
{
    //sprintf(*message, "%s", hardware_drivers->storage_controller->format_sd_card() ? "sd_card_format=true" : "sd_card_format=false");
    
    sprintf(*message, "false");
    hardware_drivers->storage_controller->format_sd_card();
    sprintf(*message, "true");
}

void CommandParser::check_camera(char* arg, char* param, char** message)
{
    sprintf(*message, "%s", hardware_drivers->camera->run_self_test());
}

bool CommandParser::process_serial_terminal()
{
    char* action;
    char* arg;
    char* param;
    char* message;
    int action_length = this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->serial_term->get_data(), 10, &action, 0, ' ');
    int arg_length;
    bool command_found = false;

    for (int i = 0; i < commands_size; i++) {
        if (strcmp(action, commands[i].name) == 0) {
            command_found = true;
            arg_length = this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->serial_term->get_data(), command_message_buffer_length, &arg, action_length, ' ');
            this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->serial_term->get_data(), command_message_buffer_length, &param, action_length + arg_length, ' ');
            message = (char*) malloc(sizeof(char) * command_message_buffer_length);
            (this->*(commands[i].function))(arg, param, &message);
            this->hardware_drivers->serial_term->print(message);
            this->hardware_drivers->serial_term->print("\r\n");
            free(message);
            free(arg);
            free(param);
        }
    }
    free(action);
    if (!command_found) {
        this->hardware_drivers->serial_term->println("Command not found.");
    }
    return command_found;    
}

void CommandParser::process_web_api(const char* command_name, const char* arg, const char* param, char** message)
{
    bool command_found = false;

    for (int i = 0; i < commands_size; i++) {
        if (strcmp(command_name, this->commands[i].name) == 0) {
            (this->*(commands[i].function))((char *)arg, (char *)param, message);
            command_found = true;
        }
    }

    if (!command_found) {
        sprintf(*message, "Command not found. >%s", command_name);
    }

}

void CommandParser::process_tcp_api()
{
    char* action;
    char* arg;
    char* param;
    char* message;
    int action_length = this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->wifi_radio->get_server_data(), 10, &action, 0, ' ');
    int arg_length;
    bool command_found = false;

    for (int i = 0; i < commands_size; i++) {
        if (strcmp(action, commands[i].name) == 0) {
            command_found = true;
            arg_length = this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->wifi_radio->get_server_data(), command_message_buffer_length, &arg, action_length, ' ');
            this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->wifi_radio->get_server_data(), command_message_buffer_length, &param, action_length + arg_length, ' ');
            message = (char*) malloc(sizeof(char) * command_message_buffer_length);
            (this->*(commands[i].function))(arg, param, &message);
            this->hardware_drivers->wifi_radio->send_server_data(message, command_message_buffer_length);
            free(message);
            free(arg);
            free(param);
        }
    }
    if (!command_found) {
        this->hardware_drivers->wifi_radio->send_server_data((char *)"Command not found.", 18);
    }
    free(action);
}
