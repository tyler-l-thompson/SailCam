/*
  Author: Tyler Thompson
  Date: Aug 21, 2021
  Description: SailCam MK5 Firmware
*/

#include <Arduino.h>
#include <hardware/drivers.h>
#include <web_server.h>
#include <settings/system_configuration.h>
#include <util/command_parser.h>

/* Function Prototypes */
bool is_capture_required();

char *stack_start;
// uint32_t loop_counter = 0;
uint32_t last_sys_time = 0;
uint32_t now_sys_time = 0;
char file_name[25];

WebServer* web_server;
CommandParser* command_parser;

DateTime* timestamp;

HardwareDrivers* hardware_drivers;

CameraState last_cam_state = SENSOR_POWER_DOWN;
bool cam_capture_required = false;

void setup() 
{
    char stack;
    stack_start = &stack;

    hardware_drivers = new HardwareDrivers();

    // initialize LED
    hardware_drivers->status_led = new StatusLED();

    // initalize serial terminal
    hardware_drivers->serial_term = new SerialTerminal(initial_hardware_serial_baud_rate);
    hardware_drivers->serial_term->debug_printf("Initial Baud: %d\r\n", initial_hardware_serial_baud_rate);

    // initialize OLED display
    hardware_drivers->old_display = new OledDisplay();

    // initialize RTC
    timestamp = new DateTime();
    hardware_drivers->system_clock = new Clock();
    *timestamp = hardware_drivers->system_clock->get_time();

    // initalize battery measurement device
    hardware_drivers->battery_management = new BatteryManagement();

    // initialize SD card and read config.ini
    hardware_drivers->storage_controller = new Storage(timestamp);
    hardware_drivers->storage_controller->log_data_point(*timestamp, (char *) "Booting up...");
    hardware_drivers->storage_controller->read_settings_file(hardware_drivers->serial_term);
    hardware_drivers->serial_term->debug_printf("New Baud will be: %d\r\n", hardware_drivers->storage_controller->get_system_configuration()->get_setting("baud_rate")->get_value_int());
    hardware_drivers->serial_term->reinitialize
    (
        hardware_drivers->storage_controller->get_system_configuration()->get_setting("baud_rate")->get_value_int()
    );
    hardware_drivers->storage_controller->print_configuration(hardware_drivers->serial_term);

    hardware_drivers->old_display->writef("SD Card: %s", hardware_drivers->storage_controller->is_card_connected() ? "True" : "False");
    hardware_drivers->old_display->update();
    hardware_drivers->old_display->set_sleep_time(hardware_drivers->storage_controller->get_system_configuration()->get_setting("display_sleep_time")->get_value_int());
    
    // write timestamp
    hardware_drivers->serial_term->debug_printf("System Time: %s\r\n", hardware_drivers->storage_controller->get_formatted_timestamp());
    hardware_drivers->old_display->write(hardware_drivers->storage_controller->get_formatted_timestamp());
    hardware_drivers->old_display->update();

    // initalize wifi
    hardware_drivers->wifi_radio = new Wifi
    (
        hardware_drivers->storage_controller->get_system_configuration()->get_setting("wifi_ssid")->get_value_str(),
        hardware_drivers->storage_controller->get_system_configuration()->get_setting("wifi_psk")->get_value_str(),
        hardware_drivers->storage_controller->get_system_configuration()->get_setting("tcp_port")->get_value_int(),
        (WIFI_MODE) hardware_drivers->storage_controller->get_system_configuration()->get_setting("wifi_mode")->get_value_int(),
        (TCP_SERVER_MODE) hardware_drivers->storage_controller->get_system_configuration()->get_setting("tcp_mode")->get_value_int()
    );

    // initalize camera
    hardware_drivers->camera = new Camera(hardware_drivers->storage_controller);
    char* cam_self_test = (char*) malloc(50);
    hardware_drivers->old_display->writef("CAM: %s", hardware_drivers->camera->run_self_test(&cam_self_test) ? "True" : "False");
    hardware_drivers->old_display->update();
    hardware_drivers->serial_term->debug_printf("%s\r\n", cam_self_test);
    hardware_drivers->storage_controller->log_data_point(*timestamp, cam_self_test);
    free(cam_self_test);

    // initialize API command parser
    command_parser = new CommandParser(hardware_drivers, stack_start);

    // initalize web server
    web_server = new WebServer(hardware_drivers, command_parser);

    // If in wifi client mode, wait for wifi to connect. Blink led based on successful/unsuccessful connection
    if (hardware_drivers->wifi_radio->get_wifi_mode() == WIFI_CLIENT) {
        int timeout = 0;
        hardware_drivers->serial_term->debug_print("Connecting to WiFi...");
        hardware_drivers->old_display->write("Connecting to WiFi...");
        hardware_drivers->old_display->update();
        while (hardware_drivers->wifi_radio->get_connection_status() != WL_CONNECTED && timeout < wifi_client_connect_timeout) {
            delay(500);
            timeout += 500;
            hardware_drivers->serial_term->debug_print(".");
        }
        if (hardware_drivers->wifi_radio->get_connection_status() == WL_CONNECTED) {
            hardware_drivers->status_led->info();
            hardware_drivers->serial_term->debug_println(" WiFi Connected.");
            hardware_drivers->old_display->write("WiFi Connected.");
            hardware_drivers->old_display->update();
        } else {
            hardware_drivers->status_led->error();
            hardware_drivers->serial_term->debug_println(" WiFi Failed to Connect.");
            hardware_drivers->old_display->write("WiFi Failed to Connect.");
            hardware_drivers->old_display->update();
            hardware_drivers->storage_controller->log_data_point(*timestamp, (char*) "Timeout reached connecting to wifi.");
        }
    }

    hardware_drivers->old_display->write(hardware_drivers->wifi_radio->get_ip_address().toString().c_str());
    hardware_drivers->old_display->update();

    // boot complete
    hardware_drivers->storage_controller->log_data_point(*timestamp, (char *) "Boot complete!");
    hardware_drivers->serial_term->debug_println("Boot Complete!");
    hardware_drivers->old_display->write("Boot Complete!");
    hardware_drivers->old_display->update();
    hardware_drivers->status_led->blink(2, 250);
}  // end setup()

void loop() 
{
    *timestamp = hardware_drivers->system_clock->get_time();

    // run auto capture
    if ((hardware_drivers->storage_controller->get_system_configuration()->get_setting("capture_mode")->get_value_int() == 1) && (is_capture_required())) {
        hardware_drivers->camera->capture_image(*timestamp);
    }

    // process the camera
    hardware_drivers->camera->process();
    if (hardware_drivers->camera->get_state() != last_cam_state)
    {
        // print timestamp
        hardware_drivers->serial_term->debug_printf
        (
            "System Time: %02d/%02d/%04d %02d:%02d:%02d\r\n", 
            timestamp->month(),
            timestamp->day(),  
            timestamp->year(), 
            timestamp->hour(), 
            timestamp->minute(), 
            timestamp->second()
        );

        // print camera state
        hardware_drivers->serial_term->debug_printf("Camera State: %d\r\n", hardware_drivers->camera->get_state());
        if (hardware_drivers->camera->get_state() == SENSOR_POWER_DOWN) // if camera just entered power down, increment capture_count
        {
            hardware_drivers->storage_controller->increment_counter((char*) "capture_count", true);
            hardware_drivers->serial_term->debug_printf("%s\r\n", hardware_drivers->camera->get_last_save_file_name());
            cam_capture_required = false;
        }
        else if (hardware_drivers->camera->get_state() == ERROR_HANDLE) // if camera enters error state, log error
        {
            hardware_drivers->storage_controller->increment_counter((char*) "error_count", true);
            hardware_drivers->serial_term->debug_printf("CAMERA ERROR!!\r\n");
            hardware_drivers->storage_controller->log_data_point(*timestamp, (char *) "Camera error! ", true);
            hardware_drivers->storage_controller->log_error_code(hardware_drivers->camera->get_error_reason());
            hardware_drivers->old_display->wake_up();
        }
        
        last_cam_state = hardware_drivers->camera->get_state();
    }
    
    now_sys_time = system_get_time();
    if ((now_sys_time - last_sys_time) > 500000) { // run every ~500ms

        // update oled display
        hardware_drivers->old_display->set_mode((DisplayMode) hardware_drivers->storage_controller->get_system_configuration()->get_setting("display_mode")->get_value_int());
        hardware_drivers->old_display->write_overview(
            *timestamp, 
            hardware_drivers->wifi_radio->get_ip_address(), 
            hardware_drivers->storage_controller->get_system_configuration()->get_setting("wifi_mode")->get_value_int(),
            hardware_drivers->storage_controller->get_system_configuration()->get_setting("capture_count")->get_value_int(),
            hardware_drivers->storage_controller->get_system_configuration()->get_setting("capture_mode")->get_value_int(),
            hardware_drivers->storage_controller->get_system_configuration()->get_setting("capture_interval")->get_value_int(),
            hardware_drivers->storage_controller->get_system_configuration()->get_setting("error_count")->get_value_int(),
            hardware_drivers->battery_management->get_battery_volts());
        hardware_drivers->old_display->update();

        // set dim base on hour
        hardware_drivers->old_display->dim((timestamp->hour() > dim_begin_hour) || (timestamp->hour() < dim_end_hour));

        last_sys_time = now_sys_time;
    }
    
    // handle wifi interfaces
    if (hardware_drivers->wifi_radio->get_wifi_mode() != WIFI_DISABLED) {
        // handle web server clients
        web_server->handle_client();

        // process API data on the TCP port
        if (hardware_drivers->wifi_radio->get_tcp_mode() == TCP_SERVER_ENABLED)
        {
            hardware_drivers->wifi_radio->read_server_data();
            if (hardware_drivers->wifi_radio->new_data_available()) {
                command_parser->process_tcp_api();
            }
        }
    }

    // read data from the serial terminal, process API commands if needed
    hardware_drivers->serial_term->read_data();
    if (hardware_drivers->serial_term->new_data_available()) {
        hardware_drivers->serial_term->debug_printf(">%s\r\n", hardware_drivers->serial_term->get_data());
        command_parser->process_serial_terminal();
    }

    /* reboot if requested */
    if (hardware_drivers->camera->is_reboot_requested() || 
        command_parser->is_reboot_requested())
    {
        hardware_drivers->storage_controller->log_data_point(*timestamp, (char*)"Reboot requested!");
        ESP.restart();
    }

    yield();
}  // end loop()

bool is_capture_required()
{
    if ((cam_capture_required == false) && 
        (timestamp->secondstime() % (uint32_t)hardware_drivers->storage_controller->get_system_configuration()->get_setting("capture_interval")->get_value_int() == 0)) 
    {
        hardware_drivers->serial_term->debug_printf("CAPTURE REQUIRED\r\n");
        cam_capture_required = true;
        return true;
    } 
    else 
    {
        return false;
    }
}