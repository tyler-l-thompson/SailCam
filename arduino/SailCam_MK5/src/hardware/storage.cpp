
#include <hardware/storage.h>

Storage::Storage(DateTime* timestamp)
{
    set_log_paths(*timestamp);
    set_formatted_timestamp(*timestamp);
    pinMode(sd_card_chip_select, OUTPUT);
    digitalWrite(sd_card_chip_select, LOW);
    this->card_connected = SD.begin(sd_card_chip_select, SD_SCK_MHZ(12));
    sdfat::SdFile::dateTimeCallback(sd_datetime_callback);
    this->system_configuration = new SystemConfiguration();
    this->log_file = new sdfat::File32();
}

Storage::~Storage()
{

    delete this->system_configuration;
    this->log_file = NULL;
}

void Storage::set_log_paths(DateTime timestamp)
{
    snprintf(log_folder_path, 11 + log_folder_base_dir_length, "%s/%04d/%02d", log_folder_base_dir, timestamp.year(), timestamp.month());
    snprintf(log_file_name, 21, "%04d_%02d_%02d.txt", timestamp.year(), timestamp.month(), timestamp.day());
    snprintf(log_file_path, 32 + log_folder_base_dir_length, "%s/%s", log_folder_path, log_file_name);
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
        this->card_connected = SD.begin(sd_card_chip_select);
    }
    return this->card_connected;
}

bool Storage::is_card_connected()
{
    return card_connected;
}

bool Storage::log_data_point(DateTime timestamp, char* data, bool more_to_log)
{
    if (!this->card_connected)
    {
        return false;
    }

    check_directory(log_folder_path);
    *(this->log_file) = SD.open(log_file_path, sdfat::O_WRITE | sdfat::O_CREAT);
    this->log_file->seek(this->log_file->size());
    
    set_formatted_timestamp(timestamp);
    this->log_file->print(formatted_timestamp);
    this->log_file->print(",");
    this->log_file->print(data);
    if (!more_to_log)
    {
        this->log_file->print("\r\n");
    }
    this->log_file->close();
    return true;
}

void Storage::log_error(const char* data)
{
    if (!this->card_connected)
    {
        return;
    }

    check_directory(log_folder_path);
    *(this->log_file) = SD.open(log_file_path, sdfat::O_WRITE | sdfat::O_CREAT);
    this->log_file->seek(this->log_file->size());
    this->log_file->print("ERROR: ");
    this->log_file->print(data);
    this->log_file->print("\r\n");
    this->log_file->close();
}

void Storage::log_error_code(int code)
{
    if (!this->card_connected)
    {
        return;
    }

    check_directory(log_folder_path);
    *(this->log_file) = SD.open(log_file_path, sdfat::O_WRITE | sdfat::O_CREAT);
    this->log_file->seek(this->log_file->size());
    this->log_file->print("ERROR_CODE: ");
    this->log_file->print(code);
    this->log_file->print("\r\n");
    this->log_file->close();
}

bool Storage::directory_exists(char* dir_name)
{
    return SD.exists(dir_name);
}

bool Storage::check_directory(char* dir_name)
{
    if (!SD.exists(dir_name)) {
        return SD.mkdir(dir_name);
    }
    return true;
}

SystemConfiguration* Storage::get_system_configuration()
{
    return this->system_configuration;
}

sdfat::File32 Storage::open_file(char* file_path, uint8_t mode)
{
    return this->SD.open(file_path, mode);
}

sdfat::File32 Storage::open_file(const char* file_path, uint8_t mode)
{
    return this->SD.open(file_path, mode);
}

int Storage::get_file_line_count(sdfat::File32 working_file)
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

bool Storage::format_sd_card(sdfat::print_t* pr)
{   
    return this->SD.format(pr);
}

// gets the time from the RTC for timestamping files
// not efficient to initialize RTC again, needs improvement
void sd_datetime_callback(uint16_t* date, uint16_t* time) 
{
    DS1672* system_clock = new DS1672();
    DateTime now = system_clock->get_time();
    
    *date = sdfat::FAT_DATE(now.year(), now.month(), now.day());
    *time = sdfat::FAT_TIME(now.hour(), now.minute(), now.second());
    system_clock->~DS1672();
    now.~DateTime();
    delete system_clock;
}

void Storage::read_settings_file(SerialTerminal* serial_term)
{
    sdfat::File32 setting_file;
    char key_buffer[file_buffer_size];
    char value_buffer[file_buffer_size];
    int key_size, value_size, line_count;

    if (this->is_card_connected()) {
        serial_term->debug_println("Card connected.");
        setting_file.open(system_configuration_path);
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
            this->write_settings_defaults();
            serial_term->debug_printf("%s not found. A new file was created with default system configuration values.", system_configuration_path);
        }
        setting_file.close();

        // increment boot_count
        this->system_configuration->increment_counter((char*) "boot_count");
        this->write_settings_file();

    } else {  // No SD card connected, using defaults.
        serial_term->debug_println("SD card not connected, default system configuration values will be used.");
    }
}

void Storage::write_settings_defaults()
{
    sdfat::File32 setting_file = this->open_file(system_configuration_path, sdfat::O_WRITE | sdfat::O_CREAT | sdfat::O_TRUNC);
    for (int i = 0; i < settings_length; i++) {
        setting_file.printf("%s=%s\r\n", this->get_system_configuration()->get_settings_defaults_key(i), this->get_system_configuration()->get_settings_defaults_value(i));
    }
    setting_file.close();
}

void Storage::print_configuration(char** message_buf)
{
    int pointer = 0;
    for (int i = 0; i < this->get_system_configuration()->get_settings_length(); i++) {
        pointer += snprintf
        (
            (*message_buf) + pointer, 
            command_message_buffer_length-pointer, 
            "%s=%s\r\n", 
            this->get_system_configuration()->get_settings_defaults_key(i),
            ((strcmp(this->get_system_configuration()->get_settings_defaults_key(i), "wifi_psk") == 0) ? "*****" : this->get_system_configuration()->get_setting(this->get_system_configuration()->get_settings_defaults_key(i))->get_value_str())
        );
    }
    (*message_buf)[pointer-1] = '\0';
}

void Storage::print_configuration(SerialTerminal* serial_term)
{
    serial_term->debug_printf("\r\n############ Begin System Configuration #############\r\n\r\n");
    for (int i = 0; i < this->get_system_configuration()->get_settings_length(); i++) {
        serial_term->debug_printf
        (
            " %30s = %s\r\n", 
            this->get_system_configuration()->get_settings_defaults_key(i), 
            ((strcmp(this->get_system_configuration()->get_settings_defaults_key(i), "wifi_psk") == 0) ? "*****" : this->get_system_configuration()->get_setting(this->get_system_configuration()->get_settings_defaults_key(i))->get_value_str())
        );
    }
    serial_term->debug_printf("\r\n############  End System Configuration  #############\r\n\r\n");
}

/**
 * Writes current system configuration values in memory to the system configuration file on the SD card
 **/
int Storage::write_settings_file(char** message_buf)
{
    sdfat::File32 settings_file;
    this->check_and_reconnect_card();
    if (this->is_card_connected()) {
        settings_file.open(system_configuration_path, sdfat::O_WRITE);
        if (settings_file) {
            for (int i = 0; i < this->get_system_configuration()->get_settings_length(); i++) {
                settings_file.printf("%s=%s\r\n", this->get_system_configuration()->get_settings_defaults_key(i), this->get_system_configuration()->get_setting(this->get_system_configuration()->get_settings_defaults_key(i))->get_value_str());
            }
            settings_file.close();
            strcpy(*message_buf, "System configuration file written successfully.\r\n");
            return 0;
        } else {
            settings_file.close();
            strcpy(*message_buf, "Failed to open settings file.\r\n");
            return 1;
        }
    } else {
        this->card_connected = false;
        strcpy(*message_buf, "SD card not connected.\r\n");
        return 2;
    }
}

int Storage::write_settings_file()
{
    sdfat::File32 settings_file;
    this->check_and_reconnect_card();
    if (this->is_card_connected()) {
        settings_file.open(system_configuration_path, sdfat::O_WRITE);
        if (settings_file) {
            for (int i = 0; i < this->get_system_configuration()->get_settings_length(); i++) {
                settings_file.printf("%s=%s\r\n", this->get_system_configuration()->get_settings_defaults_key(i), this->get_system_configuration()->get_setting(this->get_system_configuration()->get_settings_defaults_key(i))->get_value_str());
            }
            settings_file.close();
            return 0;
        } else {
            settings_file.close();
            return 1;
        }
    } else {
        this->card_connected = false;
        return 2;
    }
}

uint8_t Storage::get_sd_card_type()
{
    return this->SD.fatType();
}

int Storage::get_free_space()
{
    return 0;
}

int Storage::list_directory(sdfat::File32 dir, char** buffer, uint32_t buffer_length, uint32_t start_pos, const char* prefix, const char* suffix, bool links, const char* link_root)
{
    // sdfat::File32 start_dir;
    // int buff_index = start_pos;
    // while (start_dir.openNext(&dir, sdfat::O_RDONLY)) {
    //     buff_index += start_dir.printFileSize((sdfat::print_t*) *buffer);
    //     buff_index += start_dir.printModifyDateTime((sdfat::print_t*) *buffer);
    //     buff_index += snprintf(*buffer, buffer_length - buff_index, " ");
    //     buff_index += start_dir.printName((sdfat::print_t*) *buffer);

    //     if (start_dir.isDir()) {
    //         buff_index += snprintf(*buffer, buffer_length - buff_index, "/");
    //     }
    //     buff_index += snprintf(*buffer, buffer_length - buff_index, "\r\n");
    //     start_dir.close();
    // }

    if (start_pos >= buffer_length)
    {
        return 0;
    }

    sdfat::File32 last_entry;
    sdfat::File32 start_dir = dir;
    uint32_t buff_index = start_pos;
    char file_name_buf[50];
    char dir_name_buf[50];

    dir.getName(dir_name_buf, 50);

    // up link
    if (links)
    {
        /* current path */
        buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "<div class='title'>File Browser</div><div class='image_view_path'>%s</div>", &(link_root[19]));

        
        if ((strcmp(link_root, "/file_browser?file=/") != 0) && (strcmp(link_root, "/file_browser") != 0))
        {
            char up_dir[50];
            snprintf(up_dir, strlen(link_root) - strlen(dir_name_buf), "%s", link_root);
            buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "<div class='image_view'><a href='%s'>^up</a></div>", up_dir);
        }
    }

    for (sdfat::File32 entry = start_dir.openNextFile(sdfat::O_READ); entry && (buff_index < buffer_length - 50); entry = start_dir.openNextFile(sdfat::O_READ)) 
    {
        uint32_t idx_at_start = buff_index;
        // get the file/directory name
        entry.getName(file_name_buf, 50);

        // skip system volume information
        if (strcmp(file_name_buf, "System Volume Information") == 0)
        {
            continue;
        }

        // file & directory prefix
        buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "%s", prefix);

        // links div start tag
        if (links)
        {
             buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "<div class='image_view'>");
        }

        // directory prefix
        if (entry.isDirectory()) 
        {
            buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, ">");
        }

        // file path link start tag
        if (links)
        {
            buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "%s%s%s%s%s", "<a href='", link_root, file_name_buf, entry.isDirectory() ? "/" : "", "'>");
        }

        // file/directory name
        buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "%s", file_name_buf);
        
        // file path link end tag
        if (links)
        {
            buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "%s", "</a>");
        }

        // directory suffix and recursion
        if (entry.isDirectory()) 
        {
            buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "%s", suffix); 
            // last_entry = entry;
            // buff_index += list_directory(last_entry, buffer, buffer_length, buff_index, "*", suffix);
            // last_entry.close();
        } 

        // file size and suffix
        else if (entry.isFile())
        {
            buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, " - %.2fKB%s", (double) entry.size() / 1024, suffix);
        }

        // file path link end tag
        if (links)
        {
            buff_index += snprintf(&(*buffer)[buff_index], buffer_length - buff_index, "%s", "</div>");
        }

        entry.close();

        if (buff_index > buffer_length)
        {
            buff_index = idx_at_start;
        }
    }
    dir.close();
    start_dir.close();
    return buff_index;
}

uint8_t Storage::get_sd_error()
{
    return this->SD.card()->errorCode();
}

bool Storage::file_exists(const char* file_path)
{
    return this->SD.exists(file_path);
}

void Storage::rm_file(char* file_path)
{
    this->SD.remove(file_path);
}

void Storage::rm_dir(char* dir_path)
{
    this->SD.rmdir(dir_path);
}

void Storage::increment_counter(char * key, bool write_back)
{
    this->system_configuration->increment_counter(key);
    if (write_back && this->card_connected)
    {
        this->write_settings_file();
    }
}

bool Storage::is_directory(char* dir_path)
{
    sdfat::File32 temp_dir = this->open_file(dir_path);
    bool is_dir = temp_dir.isDirectory();
    temp_dir.close();
    return is_dir;
}