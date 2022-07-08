#include <stddef.h>
#include <stdlib.h>
#include <Arduino.h>

#ifndef SYSTEM_SETTING_H
#define SYSTEM_SETTING_H

class SystemSetting
{
private:
    const char* default_value;
    char* value;
    bool is_value_set();
    static int str_to_int(char* value);
    static bool str_to_bool(char* value);

public:
    SystemSetting(const char* default_value);
    SystemSetting();
    ~SystemSetting();
    void set_value(char* new_value, int new_size);
    void free_value();
    void update_value(char* new_value, int new_size);
    char* get_value_str();
    int get_value_int();
    bool get_value_bool();
};

#endif
