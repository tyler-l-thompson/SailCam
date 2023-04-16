
#ifndef SERIAL_TERMINAL_H
#define SERIAL_TERMINAL_H

#include <Arduino.h>
#include <config.h>
#include <stdarg.h>

class SerialTerminal: public HardwareSerial
{
private:
    char serial_buffer[serial_buffer_length];
    bool new_data;
    int data_size;

public:
    SerialTerminal(uint32_t baud_rate);
    ~SerialTerminal();
    void reinitialize(uint32_t baud_rate);
    void debug_println(const char* data);
    void debug_print(const char* data);
    void debug_printf(const char* data, ...);
    void read_data(char terminator = '\n', bool flush_after_read = true);
    char* get_data();
    int get_data_size();
    bool new_data_available();
    int parse_string(char* source, int length, char** destination, int start_index, char separator);

};


#endif
