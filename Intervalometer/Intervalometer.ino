#include "BLEDevice.h"
#include "Button.h"
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

  if (connected) {
    if (Button_Trigger.Detect_click())
    {
      ButtonPushed();
    }
  }
  
  delay(100); // Delay a second between loops.
}
