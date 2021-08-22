
#define firmware_version "1.0"

#define sd_card_chip_select 16
#define camera_chip_select 15

#define status_led_pin 2

#define battery_measure_adc_channel A0
#define battery_measure_op_disable_pin 0
#define battery_measure_adc_resolution 1024
#define battery_max_volts 4.3
#define battery_min_volts 2.5

#define serial_buffer_length 256  // max length of stored messages
#define serial_min_read 5 // minimum number of characters that need to appear in a message for it to register
#define command_message_buffer_length 1024
#define file_buffer_size 32

#define ap_local_ip 192,168,22,2
#define ap_gateway_address 192,168,22,1
#define ap_subnet_mask 255,255,255,0

#define wifi_client_connect_timeout 30000

#define web_server_port 80

#define log_folder_base_dir "logs"
#define log_folder_base_dir_length 4

#define system_configuration_path "config.ini"

#define hardware_serial_port UART0
#define initial_hardware_serial_baud_rate 9600

#define serial_debug_messages true
