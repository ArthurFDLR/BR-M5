/*---------------------------------------------*/
/*   VARIABLE and CLASS for BLE management     */
/*---------------------------------------------*/

// Trigger option
byte BUTTON_RELEASE = 0b10000000;
byte BUTTON_FOCUS   = 0b01000000;
byte BUTTON_TELE    = 0b00100000;
byte BUTTON_WIDE    = 0b00010000;
byte MODE_IMMEDIATE = 0b00001100;
byte MODE_DELAY     = 0b00000100;
byte MODE_MOVIE     = 0b00001000;

// CANON_BLUETOOTH_REMOTE_SERVICE serviceUUID
static BLEUUID serviceUUID("00050000-0000-1000-0000-d8492fffa821");
// CANON_PAIRING_SERVICE charUUID
static BLEUUID CANON_PAIRING_SERVICE("00050002-0000-1000-0000-d8492fffa821");
// CANON_SHUTTER_CONTROL_SERVICE charUUID
static BLEUUID CANON_SHUTTER_CONTROL_SERVICE("00050003-0000-1000-0000-d8492fffa821");

// BLE VARIABLES
static BLEAddress *pServerAddress;
static boolean doConnect = false;

static BLERemoteCharacteristic* pRemoteCharacteristic_Pairing;
static BLERemoteCharacteristic* pRemoteCharacteristic_Trigger;
static BLEClient*  pClient  = BLEDevice::createClient();

// When the board detect a new BLE device
class advdCallback: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.println("BLE device found: ");
    String addr = advertisedDevice.getAddress().toString().c_str(); //Stock the Mac address
    Serial.println(addr);
    if (addr.equalsIgnoreCase(addrCanonEOS)) { // Comparing to our Mac Address
      Serial.println(" - Found Canon M50"); // Stopping research and autorizing connection
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
    }
  }
};

/*---------------------------------------------*/
/*       FUNCTION for BLE management           */
/*---------------------------------------------*/


// Purpose : Scanning for new BLE devices around.
//           When found -> advdCallback::OnResult
void BLE_Scan(){
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new advdCallback()); // Retrieve a Scanner and set the callback we want to use to be informed when we have detected a new device.
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30); // Specify that we want active scanning and start the scan to run for 30 seconds.
  if (doConnect){
    Serial.println(" - Scan completed");
  } else {
    Serial.println(" - Scan failed, retry ...");
    BLE_Scan();
  }
}


// Purpose : Connect to server if ready to.
//    If the flag "doConnect" is true then we have scanned for and found the desired
//    BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
//    connected we set the connected flag to be true.
void EstablishConnection(){
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
}


// Purpose : Find reference to necessary characteristics and connect to M50.
bool connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  // Connect to Canon M50 server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are using in the remote BLE server.
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

  // Finally, pairing with Canon M50
  String DEVICE_NAME2 = " " + DEVICE_NAME + " "; //Pairing message to send
  byte cmdPress[DEVICE_NAME2.length()]; // Stocking list of Bytes char of the message
  DEVICE_NAME2.getBytes(cmdPress, DEVICE_NAME2.length()); // message Parser
  cmdPress[0] = {0x03};
  pRemoteCharacteristic_Pairing->writeValue(cmdPress, sizeof(cmdPress), false); // Writing to Canon_pairing_service
  delay(500);
  Serial.println(" - Pairing message send");
}


// Purpose : Send trigger message.
void Trigger(){
  Serial.println(" - Shutter triggered");
  byte cmdByte[] = {MODE_IMMEDIATE|BUTTON_RELEASE}; // Binary OR : Concatenate Mode and Button
  pRemoteCharacteristic_Trigger->writeValue(cmdByte, sizeof(cmdByte), false);   // Set the characteristic's value to be the array of bytes that is actually a string.
  delay(600);
  //byte cmdrstByte[] = {BUTTON_RELEASE};
  pRemoteCharacteristic_Trigger->writeValue(0b00001100, sizeof(cmdByte));
/*
  // Necessary disconnection to stop triggering ... nasty solution to an unsolved protocole reverse engineering
  connected = false;
  delay(10);
  Serial.println(" - Disctonnected");
  pClient->disconnect();
  // Reconnection
  if (connectToServer(*pServerAddress)) {
    Serial.println("We are now reconnected to the BLE Server.");
    connected = true;
  }
*/
}
