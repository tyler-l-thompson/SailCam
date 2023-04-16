
#include <util/command_parser.h>

CommandParser::CommandParser(HardwareDrivers* hardware_drivers, char* stack_start)
: commands {
                {"restart", &CommandParser::restart},
                {"get_time", &CommandParser::get_time},
                {"set_time", &CommandParser::set_time},
                {"get_var", &CommandParser::get_var},
                {"set_var", &CommandParser::set_var},
                {"get_config", &CommandParser::get_config},
                {"save_config", &CommandParser::save_config},
                {"get_mem", &CommandParser::get_free_memory},
                {"get_cpu", &CommandParser::get_cpu_speed},
                {"get_bat", &CommandParser::get_battery_level},
                {"get_wifi", &CommandParser::get_wifi_status},
                {"get_firmware", &CommandParser::get_firmware_version},
                {"blink_led", &CommandParser::blink_led},
                {"check_sd_card", &CommandParser::check_sd_card},
                {"format_sd_card", &CommandParser::format_sd_card},
                {"check_cam", &CommandParser::check_camera},
                {"capture_image", &CommandParser::capture_image},
                {"write_spi_reg", &CommandParser::write_spi_reg},
                {"read_spi_reg", &CommandParser::read_spi_reg},
                {"write_i2c_reg", &CommandParser::write_i2c_reg},
                {"read_i2c_reg", &CommandParser::read_i2c_reg},
                {"ls", &CommandParser::list_directory},
                {"mkdir", &CommandParser::make_directory},
                {"cat", &CommandParser::read_file},
                {"tail", &CommandParser::tail_file},
                {"rm", &CommandParser::remove_file},
                {"upload_file", &CommandParser::upload_file},
                {"clear_cache", &CommandParser::clear_cache},
                {"save_config_defaults", &CommandParser::save_config_defaults},
                {"wake_display", &CommandParser::wake_display},
                {"reset_error_counters", &CommandParser::reset_error_counters},
                {"uptime", &CommandParser::uptime}
            }
{
    this->hardware_drivers = hardware_drivers;
    this->stack_start = stack_start;

    for (int i = 0; i < cache_data_length; i++)
    {
        cache_file_name_hash[i] = 0u;
        cache_file_data[i] = NULL;
    }
}

CommandParser::~CommandParser()
{
}

uint32_t CommandParser::get_hash(char* value)
{
    uint32_t hash = 0;
    for (uint32_t i = 0; ((i < strlen(value)) && (value[i] != '\0')); i++)
    {
        hash += 1;
        hash += (uint32_t) value[i];
        hash *= 11;
        hash %= 1000001700;
    }
    return hash;
}

/**
 * Command functions that can be called from the API
 **/

void CommandParser::restart(char* arg, char* param, char** message) 
{
    this->reboot_requested = true;
    sprintf(*message, "Rebooting...");
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
    sprintf(*message, "free_heap=%d\r\nfree_stack=%d\r\nunits=Bytes", system_get_free_heap_size(), (total_stack_size - this->stack_size()));
}

void CommandParser::get_cpu_speed(char* arg, char* param, char** message)
{
    sprintf(*message, "cpu_speed=%d", system_get_cpu_freq());
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
        "battery_percent=%0.2f%%\r\n battery_volts=%0.2fv\r\n battery_adc=%d\r\n vcc_volts=%0.2fv", 
        battery_percent, 
        battery_volts, 
        battery_adc,
        system_volts
    );
}

void CommandParser::get_wifi_status(char* arg, char* param, char** message)
{
    bool client_status = ((this->hardware_drivers->wifi_radio->get_tcp_mode() == TCP_SERVER_ENABLED) && hardware_drivers->wifi_radio->is_client_connected());
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
    sprintf(*message, "count=%d\r\n delay=%dms", blink_count, blink_delay);
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
    sprintf(*message, "sd_card_connected=%s\r\n sd_card_type=%d\r\n error=%d", hardware_drivers->storage_controller->check_and_reconnect_card() ? "true" : "false", sd_type, hardware_drivers->storage_controller->get_sd_error());
}

void CommandParser::format_sd_card(char* arg, char* param, char** message)
{
    bool success = hardware_drivers->storage_controller->format_sd_card();
    sprintf(*message, "format_success=%s", success ? "True" : "False");
}

void CommandParser::check_camera(char* arg, char* param, char** message)
{
    hardware_drivers->camera->run_self_test(message);
}

void CommandParser::capture_image(char* arg, char* param, char** message)
{
    this->hardware_drivers->old_display->clear();
    this->hardware_drivers->old_display->print(oled_capture_message);
    this->hardware_drivers->camera->capture_image(hardware_drivers->system_clock->get_time());
    sprintf(*message, "Capture started.");
    this->hardware_drivers->old_display->clear();
    this->hardware_drivers->old_display->wake_up();
}

void CommandParser::write_spi_reg(char* arg, char* param, char** message)
{
    uint8_t addr, data, read_back;
    addr = strtol(arg, NULL, 16);
    data = strtol(param, NULL, 16);
    this->hardware_drivers->camera->cam->CS_LOW();
    this->hardware_drivers->camera->cam->write_reg(addr, data);
    read_back = this->hardware_drivers->camera->cam->read_reg(addr);
    sprintf(*message, "addr=%02x\r\n write=%02x\r\n read=%02x", addr, data, read_back);
    this->hardware_drivers->camera->cam->CS_HIGH();
}

void CommandParser::read_spi_reg(char* arg, char* param, char** message)
{    
    uint8_t addr, read_back;
    addr = strtol(arg, NULL, 16);
    this->hardware_drivers->camera->cam->CS_LOW();
    read_back = this->hardware_drivers->camera->cam->read_reg(addr);
    sprintf(*message, "addr=%02x\r\n read=%02x", addr, read_back);
    this->hardware_drivers->camera->cam->CS_HIGH();
}

void CommandParser::write_i2c_reg(char* arg, char* param, char** message)
{
    uint16_t addr;
    uint8_t data, read_back;
    addr = strtol(arg, NULL, 16);
    data = strtol(param, NULL, 16);
    this->hardware_drivers->camera->cam->wrSensorReg16_8(addr, data);
    this->hardware_drivers->camera->cam->rdSensorReg16_8(addr, &read_back);
    sprintf(*message, "addr=%02x\r\n write=%02x\r\n read=%02x", addr, data, read_back);
}

void CommandParser::read_i2c_reg(char* arg, char* param, char** message)
{
    uint16_t addr;
    uint8_t read_back;
    addr = strtol(arg, NULL, 16);
    this->hardware_drivers->camera->cam->rdSensorReg16_8(addr, &read_back);
    sprintf(*message, "addr=%02x\r\n read=%02x", addr, read_back);
}

void CommandParser::list_directory(char* arg, char* param, char** message)
{
    if (!this->hardware_drivers->storage_controller->is_card_connected()) {
        sprintf(*message, "SD card not connected.\r\n");
        return;
    }

    if (strlen(arg) == 0) {
        this->hardware_drivers->storage_controller->list_directory(hardware_drivers->storage_controller->open_file("/"), message, serial_buffer_length, 0, "", "\r\n");
    } else {
        if (hardware_drivers->storage_controller->directory_exists(arg)) {
            this->hardware_drivers->storage_controller->list_directory(hardware_drivers->storage_controller->open_file(arg), message, serial_buffer_length, 0, "", "\r\n");
        } else {
            sprintf(*message, "Directory %s doesn't exist.\r\n", arg);
        }
    }
}

void CommandParser::make_directory(char* arg, char* param, char** message)
{
    if (!this->hardware_drivers->storage_controller->is_card_connected()) {
        sprintf(*message, "SD card not connected.\r\n");
        return;
    }
    if (strlen(arg) == 0) {
        sprintf(*message, "Pass a directory as an argument.\r\n");
        return;
    }
    sprintf(*message, "%s\r\n", this->hardware_drivers->storage_controller->check_directory(arg) ? "Directory successfully created." : "Failed to create directory.");
}

void CommandParser::read_file(char * arg, char * param, char ** message)
{
    /* check that the sd card is connected */
    if (!this->hardware_drivers->storage_controller->is_card_connected())
    {
        sprintf(*message, "Failed to open file. SD card not connected.");
        return;
    }

    bool cache_hit = false;
    int cache_index = this->cache_idx;

    /* process arg checksum */
    uint32_t arg_check_sum = get_hash(arg);
    if (arg_check_sum == 0)
    {
        sprintf(*message, "Invalid argument.");
        return;
    }

    /* search cache */
    this->hardware_drivers->serial_term->debug_printf("searching cache for %s %d\r\n", arg, arg_check_sum);
    for (int i = 0; i < cache_data_length; i++)
    {
        if (this->cache_file_name_hash[i] == arg_check_sum) // cache hit
        {
            cache_hit = true;
            cache_index = i;
            i = cache_data_length;
            this->hardware_drivers->serial_term->debug_printf("cache hit at idx %d\r\n", cache_index);
        }
    }

    if (!cache_hit) // not in cache
    {
        this->hardware_drivers->serial_term->debug_printf("cache miss new idx %d\r\n", this->cache_idx);
        /* open the file */
        this->read_file_cache = this->hardware_drivers->storage_controller->open_file(arg, sdfat::O_READ);
        if (!this->read_file_cache)
        {
            sprintf(*message, "Failed to open file.");
            return;
        }

        /* allocate cache memory */
        uint32_t cache_size = get_safe_buffer_size(this->read_file_cache.size());
        if (this->cache_file_data[this->cache_idx] != NULL)
        {
            this->hardware_drivers->serial_term->debug_printf("free\r\n");
            free(this->cache_file_data[this->cache_idx]);
        }
        this->hardware_drivers->serial_term->debug_printf("allocating %d bytes for new cache data\r\n", cache_size);
        this->cache_file_data[this->cache_idx] = (char*) malloc(cache_size);

        /* read the file into cache */
        this->read_file_cache.readBytes(this->cache_file_data[this->cache_idx], cache_size);
        this->cache_file_data[this->cache_idx][cache_size-1] = '\0';
        this->read_file_cache.close();
        this->cache_file_size[this->cache_idx] = cache_size;

        /* store the file name check sum */
        this->hardware_drivers->serial_term->debug_printf("new cache hash %d\r\n", arg_check_sum);
        this->cache_file_name_hash[this->cache_idx] = arg_check_sum;

        /* increment cache index */
        this->cache_idx++;
        if (this->cache_idx >= cache_data_length)
        {
            this->cache_idx = 0;
        }
    }

    /* copy from cache to response */
    snprintf(*message, command_message_buffer_length, "%s", this->cache_file_data[cache_index]);
}

void CommandParser::tail_file(char * arg, char * param, char ** message)
{
    /* check that the sd card is connected */
    if (!this->hardware_drivers->storage_controller->is_card_connected())
    {
        sprintf(*message, "Failed to open file. SD card not connected.");
        return;
    }

    /* set number of characters to print */
    uint32_t tail_len = 100;
    if (strlen(param) > 0)
    {
        tail_len = atoi(param);
    }

    /* set file path */
    char* file_path = arg;
    if (strcmp(arg, "log") == 0)
    {
        file_path = this->hardware_drivers->storage_controller->get_log_path();
    }

    /* check file exists */
    if (!this->hardware_drivers->storage_controller->file_exists(file_path))
    {
        sprintf(*message, "File not found. %s", file_path);
        return;
    }

    /* open and read file */
    sdfat::File32 tail_file = this->hardware_drivers->storage_controller->open_file(file_path, sdfat::O_READ);
    uint32_t start_pos = ((tail_file.size() - tail_len) > 0) ? (tail_file.size() - tail_len) : 0;
    tail_file.seek(start_pos);
    uint32_t read_size = tail_file.readBytes(*message, tail_file.size() - start_pos);
    tail_file.close();
    (*message)[read_size - 1] = '\0';
}

void CommandParser::remove_file(char* arg, char* param, char** message)
{
    /* check that the sd card is connected */
    if (!this->hardware_drivers->storage_controller->is_card_connected())
    {
        sprintf(*message, "Failed to remove file. SD card not connected.");
        return;
    }

    /* check if file exists */
    if (!this->hardware_drivers->storage_controller->file_exists(arg))
    {
        sprintf(*message, "Failed to find file. %s", arg);
        return;
    }

    /* remove file */
    if (this->hardware_drivers->storage_controller->is_directory(arg))
    {
        this->hardware_drivers->storage_controller->rm_dir(arg);
    }
    else
    {
        this->hardware_drivers->storage_controller->rm_file(arg);
    }

    /* check if file still exists */
    if (this->hardware_drivers->storage_controller->file_exists(arg))
    {
        sprintf(*message, "Failed to remove file. %s", arg);
        return;
    }

    sprintf(*message, "File removed successfully. %s", arg);
}

void CommandParser::upload_file(char* arg, char* param, char** message)
{
    /* check that the sd card is connected */
    if (!this->hardware_drivers->storage_controller->is_card_connected())
    {
        sprintf(*message, "Failed to open file. SD card not connected.");
        return;
    }

    /* remove file if it exists */
    if (this->hardware_drivers->storage_controller->file_exists(arg))
    {
        this->hardware_drivers->storage_controller->rm_file(arg);
    }

    sdfat::File32 new_file = this->hardware_drivers->storage_controller->open_file(arg, sdfat::O_CREAT | sdfat::O_WRONLY);
    for (uint32_t i = 0; i < (strlen(param)-1) && param[i] != '\0'; i++)
    {
        new_file.write(param[i]);
    }
    new_file.close();
    sprintf(*message, "Upload complete.");
}

void CommandParser::clear_cache(char* arg, char* param, char** message)
{
    this->cache_idx = 0;
    for (int i = 0; i < cache_data_length; i++)
    {
        if (this->cache_file_name_hash[i] != 0)
        {
            free(this->cache_file_data[i]);
            this->cache_file_data[i] = NULL;
            this->cache_file_name_hash[i] = 0;
        }
    }

    sprintf(*message, "Cache cleared.");
}

void CommandParser::save_config_defaults(char* arg, char* param, char** message)
{
    this->hardware_drivers->storage_controller->write_settings_defaults();
    sprintf(*message, "System Configuration File set to defaults.\r\n");
}

void CommandParser::wake_display(char* arg, char* param, char** message)
{
    this->hardware_drivers->old_display->wake_up();
    int sleep_timer = this->hardware_drivers->storage_controller->get_system_configuration()->get_setting("display_sleep_time")->get_value_int();
    sprintf(*message, "Display woken up.\r\ndisplay_sleep_time=%dms.", sleep_timer);
}

void CommandParser::reset_error_counters(char* arg, char* param, char** message)
{
    this->hardware_drivers->storage_controller->get_system_configuration()->update_setting((char*)"boot_count", 0);
    this->hardware_drivers->storage_controller->get_system_configuration()->update_setting((char*)"error_count", 0);
    this->hardware_drivers->storage_controller->get_system_configuration()->update_setting((char*)"capture_count", 0);
    this->hardware_drivers->storage_controller->write_settings_file();
    sprintf(*message, "Counters reset.");
}

void CommandParser::uptime(char* arg, char* param, char** message)
{
    DateTime now = this->hardware_drivers->system_clock->get_time();
    DateTime boot_time = this->hardware_drivers->system_clock->get_boot_time();
    uint32_t seconds = now.secondstime() - boot_time.secondstime();
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;
    uint32_t days = hours / 24;
    seconds -= (minutes * 60);
    minutes -= (hours * 60);
    hours -= (days * 24);
    sprintf(*message, "uptime=%02d:%02d:%02d:%02d", days, hours, minutes, seconds);
}

bool CommandParser::process_serial_terminal()
{
    char* action;
    char* arg;
    char* param;
    char* message;
    int action_length = this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->serial_term->get_data(), 32, &action, 0, ' ');
    int arg_length;
    bool command_found = false;

    for (int i = 0; i < commands_size; i++) {
        if (strcmp(action, commands[i].name) == 0) {
            command_found = true;

            /* parse and allocate memory for arg */
            arg_length = this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->serial_term->get_data(), command_message_buffer_length, &arg, action_length, ' ');

            /* handle file upload */
            if (strcmp(action, "upload_file") == 0)
            {
                /* allocate maximum memory buffer for incoming data */
                uint32_t upload_file_buffer_size = get_safe_buffer_size(max_buffer_size) - command_message_buffer_length;
                this->hardware_drivers->serial_term->debug_printf("allocating %d bytes for buffer.\r\n", upload_file_buffer_size);
                param = (char *) malloc(upload_file_buffer_size);

                /* wait for data to start */
                this->hardware_drivers->serial_term->debug_printf("ready for data.\r\n");
                while (!this->hardware_drivers->serial_term->available())
                {
                    delay(1);
                }
                this->hardware_drivers->serial_term->debug_printf("reading...\r\n");
                
                /* read file data */
                int read_size = this->hardware_drivers->serial_term->readBytesUntil('$', param, upload_file_buffer_size);
                param[read_size - 1] = '\0';

                /* print back the file data */
                this->hardware_drivers->serial_term->debug_printf("%s\r\n", param);
                this->hardware_drivers->serial_term->debug_printf("data read.\r\n");
            }
            else
            {
                /* parse and allocate memory for param */
                this->hardware_drivers->serial_term->parse_string(this->hardware_drivers->serial_term->get_data(), command_message_buffer_length, &param, action_length + arg_length, '\n');
            }

            /* allocate memory for response */
            message = (char*) malloc(get_safe_buffer_size(command_message_buffer_length));

            /* execute function */
            (this->*(commands[i].function))(arg, param, &message);

            /* send response */
            this->hardware_drivers->serial_term->print(message);
            this->hardware_drivers->serial_term->print("\r\n");

            /* free memory */
            free(message);
            free(arg);
            free(param);
            i = commands_size;
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

uint32_t CommandParser::stack_size() {
    char stack;
    return (uint32_t)this->stack_start - (uint32_t)&stack;
}