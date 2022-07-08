
#include <settings/system_setting.h>

SystemSetting::SystemSetting(const char* default_value)
{
    this->default_value = default_value;
    this->value = NULL;
}

SystemSetting::SystemSetting()
{
    this->default_value = NULL;
    this->value = NULL;
}

SystemSetting::~SystemSetting()
{
    this->free_value();
}

bool SystemSetting::is_value_set()
{
    if (this->value == NULL) {
        return false;
    } else {
        return true;
    }
}

int SystemSetting::str_to_int(char* value)
{
    return atoi(value);
}

bool SystemSetting::str_to_bool(char* value)
{
    switch (value[0])
    {
    case 't':
        return true;
    case 'f':
        return false;
    default:
        return true;
    }
}

void SystemSetting::set_value(char* new_value, int new_size)
{   
    this->value = (char*) malloc(sizeof(char) * new_size + 1);
    snprintf(this->value, new_size + 1, new_value);
}

void SystemSetting::free_value()
{
    if (this->value != NULL) {
        free(this->value);
        this->value = NULL;
    }
}

void SystemSetting::update_value(char* new_value, int new_size)
{
    this->free_value();
    this->set_value(new_value, new_size);
}

char* SystemSetting::get_value_str()
{
    if (is_value_set()) {
        return this->value;
    } else {
        return (char*) this->default_value;
    }
}

int SystemSetting::get_value_int()
{
    if (is_value_set()) {
        return str_to_int(this->value);
    } else {
        return str_to_int((char*) this->default_value);
    }
}

bool SystemSetting::get_value_bool()
{
    if (is_value_set()) {
        return str_to_bool(this->value);
    } else {
        return str_to_bool((char*) this->default_value);
    }
}
