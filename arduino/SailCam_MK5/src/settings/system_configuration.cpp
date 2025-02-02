
#include <settings/system_configuration.h>

SystemConfiguration::SystemConfiguration()
: settings_defaults 
{ 
    {"baud_rate", "9600" },
    {"wifi_ssid", "SurveillanceCamera6"},
    {"wifi_psk", "hobbesthetiger"},
    {"wifi_mode", "2"}, // 0 - WIFI_ACCESS_POINT, 1 - WIFI_CLIENT, 2 - WIFI_DISABLED
    {"tcp_port", "723"},
    {"tcp_mode", "0"}, // 0 - DISABLED, 1 - ENABLED
    {"device_name", "SailCam"},
    {"capture_mode", "0"},
    {"capture_interval", "15"},
    {"cam_settings_mode", "0"}, // 0 - DEFAULT, 1 - USER
    {"display_mode", "0"}, // 0 - DISABLED, 1 - ENABLED, 2 - DISP_SLEEP_MODE
    {"display_sleep_time", "10"},
    {"boot_count", "0"},
    {"error_count", "0"},
    {"capture_count", "0"}
}
{
    read_settings_defaults();
}

SystemConfiguration::~SystemConfiguration()
{
}

int SystemConfiguration::get_settings_length() 
{
    return settings_length;
}

void SystemConfiguration::read_settings_defaults()
{
    for (int i = 0; i < settings_length; i++) {
        int key_length = strlen(settings_defaults[i][0]);
        settings_map[i].key = (char *) malloc(sizeof(char) * key_length + 1);
        snprintf(settings_map[i].key, key_length + 1, settings_defaults[i][0]);
        SystemSetting* new_setting = new SystemSetting(settings_defaults[i][1]);
        settings_map[i].value = new_setting;
    }
}

SystemSetting* SystemConfiguration::get_setting(char* key)
{
    for (int i = 0; i < settings_length; i++) {
        if (strcmp(settings_map[i].key, key) == 0) {
            return settings_map[i].value;
        }
    }
    return NULL;
}

SystemSetting* SystemConfiguration::get_setting(const char* key)
{
    for (int i = 0; i < settings_length; i++) {
        if (strcmp(settings_map[i].key, key) == 0) {
            return settings_map[i].value;
        }
    }
    return NULL;
}

void SystemConfiguration::set_setting(char* key, char* value, int value_size)
{
    SystemSetting *find_value = this->get_setting(key);
    if (find_value != NULL) {
        find_value->set_value(value, value_size);
    }
}

void SystemConfiguration::update_setting(char* key, char* value, int value_size)
{
    SystemSetting *find_value = this->get_setting(key);
    if (find_value != NULL) {
        find_value->update_value(value, value_size);
    }
}

void SystemConfiguration::update_setting(char* key, int value)
{
    SystemSetting *find_value = this->get_setting(key);
    if (find_value != NULL) {
        char * new_value = (char *) malloc(32);
        snprintf(new_value, 32, "%d", value);
        find_value->update_value(new_value, strlen(new_value));
        free(new_value);
    }
}

const char* SystemConfiguration::get_settings_defaults_key(int index)
{
    return (this->settings_defaults)[index][0];
}

const char* SystemConfiguration::get_settings_defaults_value(int index)
{
    return (this->settings_defaults)[index][1];
}

/**
 * Returns true or false based on if the 'key' exists in the default settings array.
 **/
bool SystemConfiguration::verify_setting_key(char* key)
{
    for (int i = 0; i < this->get_settings_length(); i++) {
        if (strcmp(key, settings_defaults[i][0]) == 0) {
            return true;
        }
    }
    return false;
}

void SystemConfiguration::increment_counter(char* key)
{
    this->update_setting(key, this->get_setting(key)->get_value_int() + 1);
}
