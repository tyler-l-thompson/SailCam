
#include <hardware/storage.h>

Storage::Storage(DateTime* timestamp)
{
    
    set_log_paths(*timestamp);
    set_formatted_timestamp(*timestamp);
    pinMode(sd_card_chip_select, OUTPUT);
    digitalWrite(sd_card_chip_select, LOW);
    this->card_connected = SD.begin(sd_card_chip_select);
    sdfat::SdFile::dateTimeCallback(sd_datetime_callback);
    this->system_configuration = new SystemConfiguration();
    this->log_file = new File();
}

Storage::~Storage()
{
    SD.end();
    delete this->system_configuration;
    this->log_file = NULL;
}

void Storage::set_log_paths(DateTime timestamp)
{
    snprintf(log_folder_path, 14 + log_folder_base_dir_length, "%s/%04d/%02d/%02d", log_folder_base_dir, timestamp.year(), timestamp.month(), timestamp.day());
    snprintf(log_file_name, 29, "%04d_%02d_%02d_%02d_%02d_%02d.txt", timestamp.year(), timestamp.month(), timestamp.day(), timestamp.hour(), timestamp.minute(), timestamp.second());
    snprintf(log_file_path, 43 + log_folder_base_dir_length, "%s/%s", log_folder_path, log_file_name);
}

void Storage::set_formatted_timestamp(DateTime timestamp)
{
    snprintf(formatted_timestamp, 25, "%02d/%02d/%04d %02d:%02d:%02d", timestamp.month(), timestamp.day(), timestamp.year(), timestamp.hour(), timestamp.minute(), timestamp.second());
}

char* Storage::get_formatted_timestamp()
{
    return this->formatted_timestamp;
}

bool Storage::check_and_reconnect_card()
{
    if (!this->card_connected) {
        SD.end();
        this->card_connected = SD.begin(sd_card_chip_select);
    }
    return this->card_connected;
}

bool Storage::is_card_connected()
{
    return card_connected;
}

bool Storage::log_data_point(DateTime timestamp, char* data)
{
    check_directory(log_folder_path);
    *(this->log_file) = SD.open(log_file_path, "a");
    
    if (this->log_file) {
        set_formatted_timestamp(timestamp);
        this->log_file->print(formatted_timestamp);
        this->log_file->print(",");
        this->log_file->print(data);
        this->log_file->print("\r\n");
        this->log_file->close();
        return true;
    } else {
        this->card_connected = false;
        return false;
    }
}

void Storage::check_directory(char* dir_name)
{
    if (!SD.exists(dir_name)) {
        SD.mkdir(dir_name);
    }
}

SystemConfiguration* Storage::get_system_configuration()
{
    return this->system_configuration;
}

File Storage::open_file(char* file_path, uint8_t mode)
{
    return SD.open(file_path, mode);
}

File Storage::open_file(const char* file_path, uint8_t mode)
{
    return SD.open(file_path, mode);
}

int Storage::get_file_line_count(File working_file)
{
    int line_count = 0;
    char buf;
    while (buf != EOF && working_file.available()) {
        buf = working_file.read();
        if (buf == '\n') { line_count++; }
    }
    working_file.seek(0);
    return line_count;
}

// gets the time from the RTC for timestamping files
// not efficient to initialize RTC again, needs improvement
void sd_datetime_callback(uint16_t* date, uint16_t* time) 
{
    DS1672* system_clock = new DS1672();
    DateTime now = system_clock->get_time();
    
    *date = FAT_DATE(now.year(), now.month(), now.day());
    *time = FAT_TIME(now.hour(), now.minute(), now.second());
    system_clock->~DS1672();
    now.~DateTime();
    delete system_clock;
}

void Storage::read_settings_file(SerialTerminal* serial_term)
{
    File setting_file;
    char key_buffer[file_buffer_size];
    char value_buffer[file_buffer_size];
    int key_size, value_size, line_count;

    if (this->is_card_connected()) {
        serial_term->debug_println("Card connected.");
        setting_file = this->open_file(system_configuration_path);
        if (setting_file) {
            line_count = this->get_file_line_count(setting_file);
            for (int i = 0; i < line_count; i++) {
                key_size = setting_file.readBytesUntil('=', key_buffer, file_buffer_size);
                key_buffer[key_size] = '\0';

                value_size = setting_file.readBytesUntil('\n', value_buffer, file_buffer_size);
                value_buffer[value_size] = '\0';
                if (value_buffer[value_size-1] == '\r') { value_buffer[value_size-1] = '\0'; value_size--;}

                this->get_system_configuration()->set_setting(key_buffer, value_buffer, value_size);
            }
            serial_term->debug_println("System configuration successfully read from SD card.");
      
        } else {  // config.ini not found. Using default values
            setting_file.close();
            setting_file = this->open_file(system_configuration_path, FILE_WRITE);
            write_settings_defaults(setting_file, this->get_system_configuration()->get_settings_length());
            serial_term->debug_printf("%s not found. A new file was created with default system configuration values.", system_configuration_path);
        }
        setting_file.close();
    } else {  // No SD card connected, using defaults.
        serial_term->debug_println("SD card not connected, default system configuration values will be used.");
    }
}

/**
 * Writes default values to a file that is already open
 **/
void Storage::write_settings_defaults(File settings_file, int length)
{
    for (int i = 0; i < length; i++) {
        settings_file.printf("%s=%s\r\n", this->get_system_configuration()->get_settings_defaults_key(i), this->get_system_configuration()->get_settings_defaults_value(i));
    }
}

void Storage::print_configuration(char** message_buf)
{
    int pointer = 0;
    for (int i = 0; i < this->get_system_configuration()->get_settings_length(); i++) {
        pointer += snprintf
        (
            (*message_buf) + pointer, 
            command_message_buffer_length-pointer, 
            "%s=%s,", 
            this->get_system_configuration()->get_settings_defaults_key(i),
            this->get_system_configuration()->get_setting(this->get_system_configuration()->get_settings_defaults_key(i))->get_value_str()
        );
    }
    (*message_buf)[pointer-1] = '\0';
}

void Storage::print_configuration(SerialTerminal* serial_term)
{
    serial_term->debug_printf("\r\n############ Begin System Configuration #############\r\n");
    for (int i = 0; i < this->get_system_configuration()->get_settings_length(); i++) {
        serial_term->debug_printf(" %30s = %s\r\n", this->get_system_configuration()->get_settings_defaults_key(i), this->get_system_configuration()->get_setting(this->get_system_configuration()->get_settings_defaults_key(i))->get_value_str());
    }
    serial_term->debug_printf("\r\n############  End System Configuration  #############\r\n");
}

/**
 * Writes current system configuration values in memory to the system configuration file on the SD card
 **/
void Storage::write_settings_file(char** message_buf)
{
    File settings_file;
    this->check_and_reconnect_card();
    if (this->is_card_connected()) {
        settings_file = SD.open(system_configuration_path, "w");
        if (settings_file) {
            for (int i = 0; i < this->get_system_configuration()->get_settings_length(); i++) {
                settings_file.printf("%s=%s\r\n", this->get_system_configuration()->get_settings_defaults_key(i), this->get_system_configuration()->get_setting(this->get_system_configuration()->get_settings_defaults_key(i))->get_value_str());
            }
            strcpy(*message_buf, "System configuration file written successfully.\r\n");
        } else {
            strcpy(*message_buf, "Failed to open settings file.\r\n");
        }
        settings_file.close();
    } else {
        this->card_connected = false;
        strcpy(*message_buf, "SD card not connected.\r\n");
    }
}
