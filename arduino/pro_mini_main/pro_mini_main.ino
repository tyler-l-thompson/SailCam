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

DS3231 Clock;
RTClib RTC;
DateTime datetime;

#if !defined (OV5642_MINI_5MP_PLUS)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif

#define   FRAMES_NUM    0x00 // specifies number of pictures to take
#define CS 7
#define SD_CS 4
#define led_pin 10
#define sleep_time 10000
#define sunrise_hour 6
#define sunset_hour 21
#define flash_interval 180

bool is_header = false;
uint16_t picture_count = 0;
char filename[] = "0000/00/00/00_00_00.jpg";
char year_filename[] = "0000";
char month_filename[] = "0000/00";
char day_filename[] = "0000/00/00";

File outFile;
ArduCAM myCAM(OV5642, CS);

/*
 * Funtion Definitions
 */
void blink_led(int delay_time, int blink_times);
uint8_t read_fifo_burst(ArduCAM myCAM);

void setup() {
  uint8_t vid, pid;
  uint8_t temp;

  Wire.begin();
  
  Serial.begin(9600);
  Serial.println(F("MacCam Start!"));
  
  // set the CS as an output:
  pinMode(CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  pinMode(led_pin, OUTPUT);
  digitalWrite(CS, HIGH);
  digitalWrite(SD_CS, HIGH);
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
  // flush and clear fifo
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  myCAM.OV5642_set_JPEG_size(OV5642_2592x1944);

  // check and create current year folder
  datetime = RTC.now();
  sprintf(year_filename, "%04d", datetime.year());
  if ( !SD.exists(filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(year_filename);
    SD.mkdir(year_filename);
  }

  // check and create current month folder
  sprintf(month_filename, "%04d/%02d", datetime.year(), datetime.month());
  if ( !SD.exists(filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(month_filename);
    SD.mkdir(month_filename);
  }

  // check and create current day folder
  sprintf(day_filename, "%04d/%02d/%02d", datetime.year(), datetime.month(), datetime.day());
  if ( !SD.exists(filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(day_filename);
    SD.mkdir(day_filename);
  }

  sprintf(filename, "%04d/%02d/%02d/%02d_%02d_%02d.jpg", datetime.year(), datetime.month(), datetime.day(), datetime.hour(), datetime.minute(), datetime.second());
  
  Serial.println(filename);
  
  // Start capture
  Serial.println("start capture.");

  // If night time and enough time has passed, use led flash
  if (((sunset_hour <= datetime.hour()) || (sunrise_hour >= datetime.hour())) && ((picture_count >= flash_interval) || (picture_count == 0))) {
    picture_count = 0;    
    digitalWrite(led_pin, HIGH);
    delay(200);
  }
  picture_count++;
  myCAM.start_capture();
  while ( !myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)); 
  
  digitalWrite(led_pin, LOW); // turn off flash
  Serial.println("CAM Capture Done.");
  
  read_fifo_burst(myCAM); // Save image to SD card
  
  myCAM.clear_fifo_flag(); // Clear the capture done flag

  // If day time, blink the led
  if ((sunset_hour > datetime.hour()) && (sunrise_hour < datetime.hour())) {
    blink_led(300,1);    
  }

  datetime = NULL;
  Serial.println("Sleeping 10 seconds...");
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

