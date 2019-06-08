#include "BLEDevice.h"
//#include "BLEScan.h"

class Button {
  public :
    
    void set_Pin(int pin)
    {
      _pinButton = pin;
    }

    bool Detect_click()
    {
      bool result = false;
      _stateButton = digitalRead(_pinButton);  // Quand on appuis sur le boutton ...
      if(_stateButton == HIGH && _previous == LOW && millis() - _timeButton > _debounce) {
        _timeButton = millis();
        result = true;
      }
      _previous == _stateButton;
      return result;
    }
  private :
    int _pinButton;
    int _stateLED = LOW;
    int _stateButton;
    int _previous = LOW;
    long _timeButton = 0;
    long _debounce = 300;
};//Button

Button Button_Trigger;
  
// This Device Name
String DEVICE_NAME = "ESP32";
byte BUTTON_RELEASE = 0b10000000;
byte BUTTON_FOCUS   = 0b01000000;
byte BUTTON_TELE    = 0b00100000;
byte BUTTON_WIDE    = 0b00010000;
byte MODE_IMMEDIATE = 0b00001100;
byte MODE_DELAY     = 0b00000100;
byte MODE_MOVIE     = 0b00001000;

// Adresse MAC du DSLR Canon EOS
static String addrCanonEOS = "00:9d:6b:4c:01:bd";

// CANON_BLUETOOTH_REMOTE_SERVICE serviceUUID
static BLEUUID serviceUUID("00050000-0000-1000-0000-d8492fffa821");
// CANON_PAIRING_SERVICE charUUID
static BLEUUID CANON_PAIRING_SERVICE("00050002-0000-1000-0000-d8492fffa821");
// CANON_SHUTTER_CONTROL_SERVICE charUUID
static BLEUUID CANON_SHUTTER_CONTROL_SERVICE("00050003-0000-1000-0000-d8492fffa821");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic_Pairing;
static BLERemoteCharacteristic* pRemoteCharacteristic_Trigger;

//
static BLEClient*  pClient  = BLEDevice::createClient();


// Quand ESP32 decouvre un nouveau periph
class advdCallback: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.println("BLE device found: ");
    String addr = advertisedDevice.getAddress().toString().c_str();
    Serial.println(addr);
    // On verifie que c'est notre appareil -- cf. adresse MAC
    if (addr.equalsIgnoreCase(addrCanonEOS)) {
      Serial.println("found CanonEOS");
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true; // Si c'est bien l'appareil qu'on cherche, on arrete la recherche et on autorise la connection a pServerAddress
    }
  }
};

void setup() {
  Button_Trigger.set_Pin(17);
  
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 30 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  // pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setAdvertisedDeviceCallbacks(new advdCallback());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
} // End of setup.

// This is the Arduino main loop function.
void loop() {
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) { //Si on a bien reussi a se connecter a pServerAddress
      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false; // Plus besoin d'autoriser la connection, on est connecte
  }

  
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    if (Button_Trigger.Detect_click())
    {
      ButtonPushed();
    }
  }
  
  delay(100); // Delay a second between loops.
}

void ButtonPushed(){
  Serial.println(" - Button Pushed");
  doShutter(MODE_IMMEDIATE, BUTTON_RELEASE); 
  connected = false;
  delay(10);
  pClient->disconnect();
  Serial.println(" - Disctonnected");
  // On reinitialise la connection pour eviter le bloquage en prise de photo de l'appareil ... protocol de prise de vue a definir plus precisement
  if (connectToServer(*pServerAddress)) {
    Serial.println("We are now reconnected to the BLE Server.");
    connected = true;
  }
}
