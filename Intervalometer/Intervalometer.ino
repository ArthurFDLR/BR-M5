#include "BLEDevice.h"
#include "TimeLapse_Management.h"
/*----------------------------------*/
/*   VARIABLES and INSTANTIATIONS   */
/*----------------------------------*/

// Relate to Buttons
int pinButton_TLstate = 17;
int pinButton_incDelay = 16;
int pinButton_decDelay = 5;

// Remote name
String DEVICE_NAME = "ESP32";
// Mac Address to connect to
static String addrCanonEOS = "00:9d:6b:4c:01:bd";
static boolean connected = false; //True if ready to communicate; False either

void Trigger(); //Specification need before instantiation of TimeLapse object, cf. BLE_Management.ino
TimeLapse TL(2000,Trigger); //Interval > ExposureTime + 2000

void IRAM_ATTR isr_TLstate() {
  TL.TimeLapse_ON = not TL.TimeLapse_ON;
}

void IRAM_ATTR isr_incDelay() {
  TL.TimeLapse_incDelay();
}

void IRAM_ATTR isr_decDelay() {
  TL.TimeLapse_decDelay();
}

/*-----------------------*/
/*   Arduino Processes   */
/*-----------------------*/
void setup() {
  pinMode(pinButton_TLstate, INPUT_PULLUP);
  pinMode(pinButton_incDelay, INPUT_PULLUP);
  pinMode(pinButton_decDelay, INPUT_PULLUP);
  
  attachInterrupt(pinButton_TLstate, isr_TLstate, FALLING);
  attachInterrupt(pinButton_incDelay, isr_incDelay, FALLING);
  attachInterrupt(pinButton_decDelay, isr_decDelay, FALLING);
  
  Serial.begin(115200);
  BLE_Scan();
}

void loop() {

  EstablishConnection();

  TL.TimeLapse_Trigger();
  Serial.println(TL.Interval);
  delay(100);
}
