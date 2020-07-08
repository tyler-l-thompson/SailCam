/*
 * SailCam
 * @author: Tyler Thompson
 * @description: Arduino Mini Pro firmware used for interfacing an ArduCam OV5642 with a SD card. Use arduino IDE 1.6.8 or above.
 * @date: April 16, 2019
 */

#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <SD.h>
#include "memorysaver.h"
#include <DS3231.h>

#if !defined (OV5642_MINI_5MP_PLUS)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif

#define FRAMES_NUM   0x00 // specifies number of pictures to take
#define CAM_CS       7
#define SD_CS        4
#define PWR_Relay    2
#define IR_Relay     3
#define led_pin      10
#define sleep_time   100
#define sunrise_hour 6
#define sunset_hour  21

DS3231 Clock;
RTClib RTC;
DateTime datetime;
File outFile;
ArduCAM myCAM(OV5642, CAM_CS);

bool is_header = false;
bool night_mode = false;
char filename[] = "0000/00/00/00_00_00.jpg";
char year_filename[] = "0000";
char month_filename[] = "0000/00";
char day_filename[] = "0000/00/00";


/*
 * Funtion Definitions
 */
void blink_led(int delay_time, int blink_times);
uint8_t read_fifo_burst(ArduCAM myCAM);
void set_night_mode(uint8_t Night_mode);


void setup() {
  uint8_t vid, pid;
  uint8_t temp;

  Wire.begin();
  
  Serial.begin(9600);
  Serial.println(F("MacCam Start!"));
  
  pinMode(CAM_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  pinMode(PWR_Relay, OUTPUT);
  pinMode(IR_Relay, OUTPUT);
  pinMode(led_pin, OUTPUT);
  digitalWrite(CAM_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(PWR_Relay, LOW);
  digitalWrite(IR_Relay, LOW);
  digitalWrite(led_pin, LOW);

  Clock.setClockMode(false);  // set to 24h
  
  // initialize SPI:
  SPI.begin();
  //Reset the CPLD
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100); 

  while(1){
    //Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    if(temp != 0x55)
    {
      Serial.println(F("SPI interface Error!"));
      blink_led(100, 1);
      delay(1000);
      continue;
    }else{
      Serial.println(F("SPI interface OK."));break;
    }
  }
  
  while(1){
    //Check if the camera module type is OV5642
    myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
    if ((vid != 0x56) || (pid != 0x42)){
      Serial.println(F("Can't find OV5642 module!"));
      blink_led(100, 2);
      delay(1000);
      continue;
    }else{
      Serial.println(F("OV5642 detected."));break;      
    }
  }

  //Change to JPEG capture mode and initialize the OV5640 module
  myCAM.set_format(JPEG);
  myCAM.InitCAM();
  myCAM.set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
  myCAM.clear_fifo_flag();
  myCAM.write_reg(ARDUCHIP_FRAMES, FRAMES_NUM);
  //myCAM.set_bit(ARDUCHIP_GPIO,GPIO_PWDN_MASK);

  //Initialize SD Card
  while(!SD.begin(SD_CS))
  {
    Serial.println(F("SD Card Error!"));
    blink_led(100, 3);
    delay(1000);
  }
  Serial.println(F("SD Card detected."));

  blink_led(500,2);
  
}

void loop() {
  
//  uint8_t reg_val;
//  Serial.print("DEBUG: ");
//  myCAM.rdSensorReg16_8(0x3a17,&reg_val);
//  Serial.print("Gain Base: ");
//  Serial.print(reg_val, HEX);
//  Serial.print(" ");
//  myCAM.rdSensorReg16_8(0x3a00,&reg_val);
//  Serial.print("AEC Mode: ");
//  Serial.print(reg_val, HEX);
//  Serial.print(" ");
//  myCAM.rdSensorReg16_8(0x3a02,&reg_val);
//  Serial.print("Max Exposure:");
//  Serial.print(reg_val, HEX);
//  Serial.print(" ");
//  myCAM.rdSensorReg16_8(0x3a03,&reg_val);
//  Serial.print("");
//  Serial.print(reg_val, HEX);
//  Serial.print(" ");
//  myCAM.rdSensorReg16_8(0x3a04,&reg_val);
//  Serial.print("");
//  Serial.println(reg_val, HEX);
  //myCAM.wrSensorReg16_8(0x3a00, 0x79);


  // check and create current year folder
  datetime = RTC.now();
  sprintf(year_filename, "%04d", datetime.year());
  if ( !SD.exists(year_filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(year_filename);
    SD.mkdir(year_filename);
  }

  // check and create current month folder
  sprintf(month_filename, "%04d/%02d", datetime.year(), datetime.month());
  if ( !SD.exists(month_filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(month_filename);
    SD.mkdir(month_filename);
  }

  // check and create current day folder
  sprintf(day_filename, "%04d/%02d/%02d", datetime.year(), datetime.month(), datetime.day());
  if ( !SD.exists(day_filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(day_filename);
    SD.mkdir(day_filename);
  }

  sprintf(filename, "%04d/%02d/%02d/%02d_%02d_%02d.jpg", datetime.year(), datetime.month(), datetime.day(), datetime.hour(), datetime.minute(), datetime.second());
  
  Serial.println(filename);
  
  // If day time, blink the led
  if ((sunset_hour > datetime.hour()) && (sunrise_hour < datetime.hour())) {
    if (night_mode == true) {
     set_night_mode(Night_Mode_Off); 
    }
    night_mode = false;
    //digitalWrite(PWR_Relay, HIGH); // external power off
    blink_led(300,1);    
  }
  else {
    if (night_mode == false) {
      set_night_mode(Night_Mode_On);
    }
    night_mode = true;
    //digitalWrite(PWR_Relay, LOW); //external power on
  }
  
  // flush and clear fifo
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  myCAM.OV5642_set_JPEG_size(OV5642_2592x1944);
  
  // Start capture
  Serial.println("start capture.");

  // turn IR flash on
  if (night_mode == true) {
   digitalWrite(IR_Relay, HIGH); 
  }

  delay(250);
  myCAM.start_capture();
  while ( !myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)); 
  
  delay(1000);
  
  // turn off flash
  digitalWrite(IR_Relay, LOW);
  Serial.println("CAM Capture Done.");
  
  read_fifo_burst(myCAM); // Save image to SD card
  
  myCAM.clear_fifo_flag(); // Clear the capture done flag

  delay(sleep_time);
}

/*
 * Blink the led defined by variable led_pin
 * @param delay_time The total time of one off and on cycle
 * @param blink_times Number of times to blink the led
 * @return NULL
 */
void blink_led(int delay_time, int blink_times) {
  for(int i = 0; i < blink_times; i++) {
    digitalWrite(led_pin, HIGH);
    delay(delay_time/2);
    digitalWrite(led_pin, LOW);
    delay(delay_time/2);
  }
}

/*
 * Save an image to the SD card. Modified from the ArduCam examples.
 * @param myCam ArduCam camera object
 * @return int 0 - Failed save | 1 - Successful save
 */
uint8_t read_fifo_burst(ArduCAM myCAM) {
  uint8_t temp = 0, temp_last = 0;
  uint32_t length = 0;
  static int i = 0;
  
  byte buf[256]; 
  
  length = myCAM.read_fifo_length();
  Serial.print(F("The fifo length is :"));
  Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //8M
  {
    Serial.println("Over size.");
    return 0;
  }
  if (length == 0 ) //0 kb
  {
    Serial.println(F("Size is 0."));
    return 0;
  } 
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();//Set fifo burst mode
  i = 0;
  while ( length-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    //Read JPEG data from FIFO
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {
      buf[i++] = temp;  //save the last  0XD9     
      //Write the remain bytes in the buffer
      myCAM.CS_HIGH();
      outFile.write(buf, i);    
      //Close the file
      outFile.close();
      Serial.println(F("Save Complete"));
      is_header = false;
      myCAM.CS_LOW();
      myCAM.set_fifo_burst();
      i = 0;
    }  
    if (is_header == true)
    { 
      //Write image data to buffer if not full
      if (i < 256)
       buf[i++] = temp;
      else
      {
        //Write 256 bytes image data to file
        myCAM.CS_HIGH();
        outFile.write(buf, 256);
        i = 0;
        buf[i++] = temp;
        myCAM.CS_LOW();
        myCAM.set_fifo_burst();
      }        
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      myCAM.CS_HIGH();

      outFile = SD.open(filename, FILE_WRITE);
      delay(100);
      if (! outFile)
      {
        Serial.println(F("File open failed"));
        while (1) {
          blink_led(100, 3);
          delay(1000);
        }
      }
      myCAM.CS_LOW();
      myCAM.set_fifo_burst();   
      buf[i++] = temp_last;
      buf[i++] = temp;   
    }
  }
    myCAM.CS_HIGH();
    return 1;
}

void set_night_mode(uint8_t Night_mode) {
  Serial.print("Night Mode: ");
  Serial.println(Night_mode);
  switch(Night_mode) {
    case Night_Mode_On:
      myCAM.wrSensorReg16_8(0x3a00, 0x78 );  //AEC mode
      break;
    case Night_Mode_Off:
      myCAM.wrSensorReg16_8(0x3a00, 0x7c);  //AEC mode
      break;
    default:
      break;
  }
}

