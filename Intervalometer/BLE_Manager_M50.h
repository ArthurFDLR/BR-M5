#ifndef BLE_Manager_M50_h
#define BLE_Manager_M50_h

#include <Arduino.h>
#include "BLEDevice.h"

class BLE_M50
{
  public:
    BLE_M50(String _Device_name, String _MacAdress_M50);
    void Scan();
    void Connect();

	String _Device_name;
	String _MacAdress_M50;

  private:

	  bool connectToServer(BLEAddress pAddress);

	  // CANON_BLUETOOTH_REMOTE_SERVICE serviceUUID
	  static BLEUUID serviceUUID("00050000-0000-1000-0000-d8492fffa821");
	  // CANON_PAIRING_SERVICE charUUID
	  static BLEUUID CANON_PAIRING_SERVICE("00050002-0000-1000-0000-d8492fffa821");
	  // CANON_SHUTTER_CONTROL_SERVICE charUUID
	  static BLEUUID CANON_SHUTTER_CONTROL_SERVICE("00050003-0000-1000-0000-d8492fffa821");

	  // Variable de connection au M50
	  BLEAddress *pServerAddress;
	  boolean doConnect = false;
	  boolean connected = false;
	  BLERemoteCharacteristic* pRemoteCharacteristic_Pairing;
	  BLERemoteCharacteristic* pRemoteCharacteristic_Trigger;
	  BLEClient*  pClient;
};

class advdCallback : public BLEAdvertisedDeviceCallbacks
{
public:
	void onResult(BLEAdvertisedDevice advertisedDevice);
};

#endif
