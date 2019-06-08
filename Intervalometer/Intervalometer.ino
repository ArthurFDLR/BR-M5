/**
 * 
 * CBRE_ESP32.ino
 * 
 * ESP32とBluetooth対応EOSとをペアリングしてシャッターを切る何か。
 * ArduinoのボードマネージャーからESP32 1.0.0を選ぶこと。1.0.1では動かない。
 * カメラのMACアドレスを入力すること。
 * ペアリング完了したらカメラのシャッターをセルフ10秒/リモコンにすること。
 * 
 */

#include "BLEDevice.h"
//#include "BLEScan.h"

// Relate to Button
int pinButton = 17;
int stateLED = LOW;
int stateButton;
int previous = LOW;
long timeButton = 0;
long debounce = 300;

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


bool connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  // Connect to Canon Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the Pairing characteristic in the service of the remote BLE server.
  pRemoteCharacteristic_Pairing = pRemoteService->getCharacteristic(CANON_PAIRING_SERVICE);
  if (pRemoteCharacteristic_Pairing == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(CANON_PAIRING_SERVICE.toString().c_str());
    return false;
  }
  Serial.println(" - Found Pairing characteristic");

  // Obtain a reference to the Trigger characteristic in the service of the remote BLE server.
  pRemoteCharacteristic_Trigger = pRemoteService->getCharacteristic(CANON_SHUTTER_CONTROL_SERVICE);
  if (pRemoteCharacteristic_Trigger == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(CANON_PAIRING_SERVICE.toString().c_str());
    return false;
  }
  Serial.println(" - Found Trigger characteristic");

  // Pairing with Canon EOS
  String DEVICE_NAME2 = " " + DEVICE_NAME + " "; //Message a envoyer pour Pairing
  byte cmdPress[DEVICE_NAME2.length()]; // Liste de stockage des Bytes composant le message
  DEVICE_NAME2.getBytes(cmdPress, DEVICE_NAME2.length()); // Parser
  cmdPress[0] = {0x03};
  pRemoteCharacteristic_Pairing->writeValue(cmdPress, sizeof(cmdPress), false); // ecrire sur la caracteristique Canon_pairing_service
  delay(500);
  Serial.println(" - Pairing message send");
}

bool ReconnectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  // Connect to Canon Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");

  // La recherche des characteristiques et service est inutile car deja stocke en memoire.
  
  // Pairing with Canon EOS
  String DEVICE_NAME2 = " " + DEVICE_NAME + " "; //Message a envoyer pour Pairing
  byte cmdPress[DEVICE_NAME2.length()]; // Liste de stockage des Bytes composant le message
  DEVICE_NAME2.getBytes(cmdPress, DEVICE_NAME2.length()); // Parser
  cmdPress[0] = {0x03};
  pRemoteCharacteristic_Pairing->writeValue(cmdPress, sizeof(cmdPress), false); // ecrire sur la caracteristique Canon_pairing_service
  delay(500);
  Serial.println(" - Pairing message send");
}

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
  pinMode(pinButton, INPUT);
    
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
      // connectPairing();
      connected = true;
      
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false; // Plus besoin d'autoriser la connection, on est connecte
  }

  
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    stateButton = digitalRead(pinButton);  
    if(stateButton == HIGH && previous == LOW && millis() - timeButton > debounce) {
      timeButton = millis();
      ButtonPushed();
    }
    previous == stateButton;
  }
  
  delay(100); // Delay a second between loops.
}

void ButtonPushed(){
  Serial.println(" - Button Pushed");
  doShutter(MODE_IMMEDIATE, BUTTON_RELEASE);
  connected = false;
  delay(100);
  Serial.println(" - Disctonnected");
  pClient->disconnect();
  
  if (connectToServer(*pServerAddress)) {
    Serial.println("We are now reconnected to the BLE Server.");
    connected = true;
  }
}

bool doShutter(byte mode, byte buttom ) {  // Activer le Shutter
  Serial.println(" - Shutter triggered");
  byte cmdByte[] = {mode|buttom}; // OU binaire : concatene les bits cf. reverse Engineering
  // Set the characteristic's value to be the array of bytes that is actually a string.
  pRemoteCharacteristic_Trigger->writeValue(cmdByte, sizeof(cmdByte), false);
}
