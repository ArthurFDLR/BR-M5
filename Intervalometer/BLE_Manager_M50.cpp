#include "BLE_Manager_M50.h"

BLE_M50::BLE_M50(String Device_name, String MacAdress)
{
	_Device_name = Device_name;
	_MacAdress_M50 = MacAdress;

	pClient = BLEDevice::createClient();
}

void BLE_M50::Scan()
{
	Serial.println("Starting Arduino BLE Client application...");
	BLEDevice::init("");

	// Retrieve a Scanner and set the callback we want to use to be informed when we
	// have detected a new device.  Specify that we want active scanning and start the
	// scan to run for 30 seconds.
	BLEScan* pBLEScan = BLEDevice::getScan();
	pBLEScan->setAdvertisedDeviceCallbacks(new advdCallback());
	pBLEScan->setActiveScan(true);
	pBLEScan->start(30);
}

void BLE_M50::Connect()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}

bool BLE_M50::connectToServer(BLEAddress pAddress) {
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
	cmdPress[0] = { 0x03 };
	pRemoteCharacteristic_Pairing->writeValue(cmdPress, sizeof(cmdPress), false); // ecrire sur la caracteristique Canon_pairing_service
	delay(500);
	Serial.println(" - Pairing message send");
} //connectToServer



void advdCallback::onResult(BLEAdvertisedDevice advertisedDevice)
{
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