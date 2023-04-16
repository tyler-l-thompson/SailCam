
#include <hardware/serial_terminal.h>

SerialTerminal::SerialTerminal(uint32_t baud_rate) : HardwareSerial(hardware_serial_port)
{
    new_data = false;
    begin(baud_rate);
    //setDebugOutput(true);
    println();
    flush();
}

SerialTerminal::~SerialTerminal()
{
    flush();
    end();
}

void SerialTerminal::reinitialize(uint32_t baud_rate)
{
    new_data = false;
    flush();
    end();
    begin(baud_rate);
    println();
    flush();
    this->debug_printf("%s\r\n%41s \r\n", boot_message, firmware_version);
}

void SerialTerminal::debug_println(const char* data)
{
    if (serial_debug_messages) { 
        println(data); 
        flush();
    }
}

void SerialTerminal::debug_print(const char* data)
{
    if (serial_debug_messages) { 
        print(data); 
        flush();
    }
}

void SerialTerminal::debug_printf(const char* data, ...)
{
    if (serial_debug_messages) { 
        va_list args;
        va_start(args, data);
        vprintf(data, args);
        va_end(args);
        flush();
    };
}

void SerialTerminal::read_data(char terminator, bool flush_after_read)
{
    int read_size;

    if (available() > 0) {

        read_size = readBytesUntil(terminator, this->serial_buffer, serial_buffer_length);

        if (read_size > serial_min_read && isalnum(serial_buffer[0])) {
            this->serial_buffer[read_size >= serial_buffer_length ? serial_buffer_length : read_size] = '\0';  // null terminate string read from buffer

            if (flush_after_read)
            {
                flush();
            }

            this->new_data = true;
            if ((serial_buffer[read_size-1] == '\r') || 
                (serial_buffer[read_size-1] == '\n') || 
                (serial_buffer[read_size-1] == terminator)) 
            { //remove trailing \r if it exists
                serial_buffer[read_size-1] = '\0';
                read_size--;
            }
            this->data_size = read_size;

        } else {
            if (flush_after_read)
            {
                flush();
            }
        }
    }
}

char* SerialTerminal::get_data()
{
    return serial_buffer;
}

int SerialTerminal::get_data_size()
{
    return this->data_size;
}

bool SerialTerminal::new_data_available()
{
    bool return_val = this->new_data;
    this->new_data = false;
    return return_val;
}

/**
 * Copies a section of a string to another string.
 * Copy will stop at length or separator, which ever comes first.
 * Returns the length of the new string.
 **/
int SerialTerminal::parse_string(char* source, int length, char** destination, int start_index, char separator)
{
    int new_length = 0;
    for (int i = start_index; i <= length && source[i] != separator && source[i] != '\0'; i++) {
        new_length++;
    }
    *destination = (char*) malloc(sizeof(char) * (new_length + 1));
    snprintf(*destination, new_length+1, &(source[start_index]));
    return new_length + 1;
}
