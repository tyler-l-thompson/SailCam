
#define firmware_version "MK 5.2"

#define sd_card_chip_select 0
#define camera_chip_select 16

#define status_led_pin 2

#define battery_measure_adc_channel A0
#define battery_measure_op_disable_pin 15
#define battery_measure_adc_resolution 1024
#define battery_max_volts 4.3
#define battery_min_volts 2.5

#define serial_buffer_length 1024 // max length of stored messages
#define serial_min_read 1 // minimum number of characters that need to appear in a message for it to register
#define command_message_buffer_length 8192
#define file_buffer_size 50

#define ap_local_ip 192,168,22,2
#define ap_gateway_address 192,168,22,1
#define ap_subnet_mask 255,255,255,0

#define wifi_client_connect_timeout 30000

#define web_server_port 80

#define log_folder_base_dir "logs"
#define log_folder_base_dir_length 4

#define image_base_dir "images"

#define system_configuration_path "config.ini"
#define camera_settings_path "camera_config.ini"
#define root_html_file "/api?command=cat&arg=/html/controls.html"

#define hardware_serial_port UART0
#define initial_hardware_serial_baud_rate 9600

#define serial_debug_messages true

#define min_heap_size 6128
#define total_stack_size 4048
#define max_buffer_size 10000

#define boot_message "\r\n   _____       _ _  _____                 "\
                     "\r\n  / ____|     (_) |/ ____|                "\
                     "\r\n | (___   __ _ _| | |     __ _ _ __ ___   "\
                     "\r\n  \\___ \\ / _` | | | |    / _` | '_ ` _ \\  "\
                     "\r\n  ____) | (_| | | | |___| (_| | | | | | | "\
                     "\r\n |_____/ \\__,_|_|_|\\_____\\__,_|_| |_| |_| \r\n"

#define oled_boot_message "\n _ _ o|  _ _  _ _\n"\
                          "_\\(_||| (_(_|| | |"
                          
#define oled_capture_message "          _\n"\
                             "        _|#|_\n"\
                             "       | (O) |\n"\
                             "        -----\n"

/**
 * IO Pinout
 * 0    SD CS
 * 2    IR/Status LED
 * 4    SDA
 * 5    SCL
 * 12   MISO
 * 13   MOSI
 * 14   SCK
 * 15   OpAmpEN
 * 16   CAM CS
 */

/**
 * I2C Slave Addresses
 * 0x3D - OLED Display
 * 0x78 - Camera
 * 0x68 - RTC
 */

/*

//Light Mode
#define Advanced_AWB         0
#define Simple_AWB           1
#define Manual_day           2
#define Manual_A             3
#define Manual_cwf           4
#define Manual_cloudy        5

//Color Saturation 
#define Saturation4          0
#define Saturation3          1
#define Saturation2          2
#define Saturation1          3
#define Saturation0          4
#define Saturation_1         5
#define Saturation_2         6
#define Saturation_3         7
#define Saturation_4         8

//Brightness
#define Brightness4          0
#define Brightness3          1
#define Brightness2          2
#define Brightness1          3
#define Brightness0          4 *
#define Brightness_1         5
#define Brightness_2         6
#define Brightness_3         7
#define Brightness_4         8

//Contrast
#define Contrast4            0
#define Contrast3            1
#define Contrast2            2
#define Contrast1            3
#define Contrast0            4
#define Contrast_1           5
#define Contrast_2           6
#define Contrast_3           7
#define Contrast_4           8

//Special effects
#define Antique                      0
#define Bluish                       1
#define Greenish                     2
#define Reddish                      3
#define BW                           4
#define Negative                     5
#define BWnegative                   6
#define Normal                       7
#define Sepia                        8
#define Overexposure                 9
#define Solarize                     10
#define  Blueish                     11
#define Yellowish                    12

//Hue
#define degree_180            0
#define degree_150            1
#define degree_120            2
#define degree_90             3
#define degree_60             4
#define degree_30             5
#define degree_0              6
#define degree30              7
#define degree60              8
#define degree90              9
#define degree120             10
#define degree150             11

//Exposure
#define Exposure_17_EV                    0
#define Exposure_13_EV                    1
#define Exposure_10_EV                    2
#define Exposure_07_EV                    3
#define Exposure_03_EV                    4
#define Exposure_default                  5
#define Exposure03_EV                     6
#define Exposure07_EV                     7
#define Exposure10_EV                     8
#define Exposure13_EV                     9
#define Exposure17_EV                     10

//Sharpness
#define Auto_Sharpness_default              0
#define Auto_Sharpness1                     1
#define Auto_Sharpness2                     2
#define Manual_Sharpnessoff                 3
#define Manual_Sharpness1                   4
#define Manual_Sharpness2                   5
#define Manual_Sharpness3                   6
#define Manual_Sharpness4                   7
#define Manual_Sharpness5                   8

//Mirror
#define MIRROR                              0
#define FLIP                                1
#define MIRROR_FLIP                         2
#define Normal                              7

*/