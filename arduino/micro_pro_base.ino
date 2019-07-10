// ArduCAM Mini demo (C)2017 Lee
// Web: http://www.ArduCAM.com
// This program is a demo of how to use the enhanced functions
// This demo was made for ArduCAM_Mini_5MP_Plus.
// It can  continue shooting and store it into the SD card  in JPEG format
// The demo sketch will do the following tasks
// 1. Set the camera to JPEG output mode.
// 2. Capture a JPEG photo and buffer the image to FIFO
// 3.Write the picture data to the SD card
// 5.close the file
//You can change the FRAMES_NUM count to change the number of the picture.
//IF the FRAMES_NUM is 0X00, take one photos
//IF the FRAMES_NUM is 0X01, take two photos
//IF the FRAMES_NUM is 0X02, take three photos
//IF the FRAMES_NUM is 0X03, take four photos
//IF the FRAMES_NUM is 0X04, take five photos
//IF the FRAMES_NUM is 0X05, take six photos
//IF the FRAMES_NUM is 0X06, take seven photos
//IF the FRAMES_NUM is 0XFF, continue shooting until the FIFO is full
//You can see the picture in the SD card.
// This program requires the ArduCAM V4.0.0 (or later) library and ArduCAM_Mini_5MP_Plus
// and use Arduino IDE 1.6.8 compiler or above

#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <SD.h>
#include "memorysaver.h"
#include <DS3231.h>

DS3231 Clock;
RTClib RTC;
DateTime datetime;

//This demo can only work on OV5640_MINI_5MP_PLUS or OV5642_MINI_5MP_PLUS platform.
#if !(defined (OV5640_MINI_5MP_PLUS)||defined (OV5642_MINI_5MP_PLUS))
#error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif
#define   FRAMES_NUM    0x00
// set pin 7 as the slave select for the digital pot:
#define CS 7
#define SD_CS 4
#define led 10
bool is_header = false;

char filename[] = "0000/00/00/00_00_00.jpg";
char year_filename[] = "0000";
char month_filename[] = "0000/00";
char day_filename[] = "0000/00/00";
File outFile;

ArduCAM myCAM(OV5642, CS);

void blink_led(int delay_time, int blink_times);
uint8_t read_fifo_burst(ArduCAM myCAM);

void setup() {
  // put your setup code here, to run once:
  uint8_t vid, pid;
  uint8_t temp;

  Wire.begin();
  
  Serial.begin(9600);
  Serial.println(F("MacCam Start!"));
  
  // set the CS as an output:
  pinMode(CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(CS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(led, LOW);

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
  
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  myCAM.OV5642_set_JPEG_size(OV5642_2592x1944);
  // delay(1000);

  datetime = RTC.now();
  sprintf(year_filename, "%04d", datetime.year());
  if ( !SD.exists(filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(year_filename);
    SD.mkdir(year_filename);
  }

  sprintf(month_filename, "%04d/%02d", datetime.year(), datetime.month());
  if ( !SD.exists(filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(month_filename);
    SD.mkdir(month_filename);
  }

  sprintf(day_filename, "%04d/%02d/%02d", datetime.year(), datetime.month(), datetime.day());
  if ( !SD.exists(filename) ) {
    Serial.print("Creating folder: ");
    Serial.println(day_filename);
    SD.mkdir(day_filename);
  }

  sprintf(filename, "%04d/%02d/%02d/%02d_%02d_%02d.jpg", datetime.year(), datetime.month(), datetime.day(), datetime.hour(), datetime.minute(), datetime.second());
  datetime = NULL;
  Serial.println(filename);
  
  //Start capture
  myCAM.start_capture();
  Serial.println(F("start capture."));
  
  while ( !myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)); 
  
  Serial.println(F("CAM Capture Done."));

  
  read_fifo_burst(myCAM);
  
  Serial.println("Sleeping 10 Seconds...");
  
  //Clear the capture done flag
  myCAM.clear_fifo_flag();

  blink_led(300,1);
  delay(10000);
}

void blink_led(int delay_time, int blink_times) {
  for(int i = 0; i < blink_times; i++) {
    digitalWrite(led, HIGH);
    delay(delay_time/2);
    digitalWrite(led, LOW);
    delay(delay_time/2);
  }
}

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

