/**
 * Connect through BLE protocol to a Canon DSLR to take picture
 * Intervalometer, single trigger or external trigger mode possible
 * Developed on ESP32 DevKitc_V4 with OLED display 128x64
 * 
 * author : Arthur FINDELAIR, github.com/ArthurFDLR
 * date : July 2019
 */

#include "BLEDevice.h"
#include "TimeLapse_Management.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

/*----------------------------------*/
/*   VARIABLES and INSTANTIATIONS   */
/*----------------------------------*/

//Relate to screen
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Remote name
String DEVICE_NAME = "ESP32";
// Mac Address to connect to
static String addrCanonEOS = "00:9d:6b:4c:01:bd";
static boolean connected = false; //True if ready to communicate; False either
int Remote_mode = 0; // 0=>Timelapse ; 1=>Single shot ; 2=>External trigger

void Trigger(); //Specification need before instantiation of TimeLapse object, cf. BLE_Management.ino
TimeLapse TL(400,Trigger); //Interval > ExposureTime + 200

// Relate to Buttons
int pinButton_TLstate = 16;
int pinButton_incDelay = 19;
int pinButton_decDelay = 18;
int pinButton_extTrigger = 32;
int pinButton_RemoteMode = 17;


void IRAM_ATTR isr_TLstate() {
  switch (Remote_mode)
  {
    case 0:
        TL.TimeLapse_ON = not TL.TimeLapse_ON;
        TL.Pic_count = 0;
      break;
    case 1:
        TL.SingleShot_ON = true;
      break;
  }
}

void IRAM_ATTR isr_ExtTrigger() {
  switch (Remote_mode)
  {
    case 2:
        TL.SingleShot_ON = true;
      break;
  }
}

void IRAM_ATTR isr_incDelay() {
  TL.TimeLapse_incDelay();
}

void IRAM_ATTR isr_decDelay() {
  TL.TimeLapse_decDelay();
}

void IRAM_ATTR isr_RemoteMode() {
  if (Remote_mode<2){
    Remote_mode += 1;
  } else {
    Remote_mode = 0;
  }
}


/*-----------------------*/
/*   Arduino Processes   */
/*-----------------------*/
void setup() {
  pinMode(pinButton_TLstate, INPUT_PULLUP);
  pinMode(pinButton_incDelay, INPUT_PULLUP);
  pinMode(pinButton_decDelay, INPUT_PULLUP);
  pinMode(pinButton_extTrigger, INPUT_PULLUP);
  pinMode(pinButton_RemoteMode, INPUT_PULLUP);
  
  
  attachInterrupt(pinButton_TLstate, isr_TLstate, FALLING);
  attachInterrupt(pinButton_incDelay, isr_incDelay, FALLING);
  attachInterrupt(pinButton_decDelay, isr_decDelay, FALLING);
  attachInterrupt(pinButton_extTrigger, isr_ExtTrigger, FALLING);
  attachInterrupt(pinButton_RemoteMode, isr_RemoteMode, FALLING);
  
  Screen_Initialize();
  Serial.begin(115200);
  BLE_Scan();
  
}

void loop() {
  EstablishConnection();
  
  Screen_Displayer();

  switch (Remote_mode)
  {
    case 0:
      TL.TimeLapse_Trigger();
      break;
      
    case 1:
      if (TL.SingleShot_ON){
        Trigger();
        TL.SingleShot_ON = false;
      }
      break;
      
    case 2:
      if (TL.SingleShot_ON){
        Trigger();
        TL.SingleShot_ON = false;
      }
      break;
  }
  
  TL.TimeLapse_Trigger();

  delay(100);
}
