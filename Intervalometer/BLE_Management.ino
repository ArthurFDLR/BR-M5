
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
} //connectToServer


bool doShutter(byte mode, byte buttom ) {  // Activer le Shutter
  Serial.println(" - Shutter triggered");
  byte cmdByte[] = {mode|buttom}; // OU binaire : concatene les bits cf. reverse Engineering
  // Set the characteristic's value to be the array of bytes that is actually a string.
  pRemoteCharacteristic_Trigger->writeValue(cmdByte, sizeof(cmdByte), false);
}
