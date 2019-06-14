#include "BLEDevice.h"
#include "TimeLapse_Management.h"
/*----------------*/
/*   VARIABLES    */
/*----------------*/

// Relate to Button
int pinButton = 17;
Button Button_Trigger(pinButton);


// Remote name
String DEVICE_NAME = "ESP32";
// Mac Address to connect to
static String addrCanonEOS = "00:9d:6b:4c:01:bd";
static boolean connected = false; //True if ready to communicate; False either

void Trigger();
TimeLapse TL(1.0,Trigger);

void setup() {
  pinMode(pinButton, INPUT);
  
  Serial.begin(115200);
  BLE_Scan();
}

/*-----------------------*/
/*   Arduino Processes   */
/*-----------------------*/

void loop() {

  EstablishConnection();


  if (Button_Trigger.Detect_click())
  {
    TL.TimeLapse_ON = not TL.TimeLapse_ON;
    Serial.println(TL.TimeLapse_ON);
  }

  TL.TimeLapse_Trigger();
  
  delay(100);
}
