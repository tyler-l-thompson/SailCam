
#ifndef SYSTEM_CONFIGURATION_H
#define SYSTEM_CONFIGURATION_H

#include <settings/system_setting.h>

struct SettingKeyValuePair {
    char *key;
    SystemSetting *value;
};


class SystemConfiguration
{
private:
    SettingKeyValuePair settings_map[7];
    int settings_length;
    const char* settings_defaults[7][2];
    void read_settings_defaults();

public:
    SystemConfiguration();
    ~SystemConfiguration();
    int get_settings_length();
    SystemSetting* get_setting(char* key);
    SystemSetting* get_setting(const char* key);
    void set_setting(char* key, char* value, int value_size);
    void update_setting(char* key, char* value, int value_size);
    const char** get_settings_defaults() { return *this->settings_defaults; };
    const char* get_settings_defaults_key(int index);
    const char* get_settings_defaults_value(int index);
    bool verify_setting_key(char* key);
    SettingKeyValuePair* get_settings_map() { return this->settings_map; };
};



#endif