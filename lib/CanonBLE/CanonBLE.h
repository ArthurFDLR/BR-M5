#ifndef CANONBLE_H_
#define CANONBLE_H_

#include <BLEDevice.h>
#include <Arduino.h>


// When the board detect a new BLE device
class advdCallback: public BLEAdvertisedDeviceCallbacks {
private:
    BLEAddress* paddress_to_connect;
    BLEUUID service_uuid_wanted;
    bool* pready_to_connect;
public:
    advdCallback(BLEUUID service_uuid, bool* ready_to_connect, BLEAddress* address_to_connect);
    void onResult(BLEAdvertisedDevice advertisedDevice);
};

class CanonBLE
{
private:

    // Trigger options
    const byte BUTTON_RELEASE = 0b10000000;
    const byte BUTTON_FOCUS   = 0b01000000;
    const byte BUTTON_TELE    = 0b00100000;
    const byte BUTTON_WIDE    = 0b00010000;
    const byte MODE_IMMEDIATE = 0b00001100;
    const byte MODE_DELAY     = 0b00000100;
    const byte MODE_MOVIE     = 0b00001000;

    // CANON_BLUETOOTH_REMOTE_SERVICE serviceUUID
    const BLEUUID SERVICE_UUID;
    // CANON_PAIRING_SERVICE charUUID
    const BLEUUID PAIRING_SERVICE;
    // CANON_SHUTTER_CONTROL_SERVICE charUUID
    const BLEUUID SHUTTER_CONTROL_SERVICE;

    BLEClient*  pclient  = BLEDevice::createClient();
    BLEAddress ext_device_address = BLEAddress("");
    BLERemoteService* pRemoteService;
    BLERemoteCharacteristic* pRemoteCharacteristic_Pairing;
    BLERemoteCharacteristic* pRemoteCharacteristic_Trigger;

    bool ready_to_connect = false;
    String device_name = "";

public :
    CanonBLE(String name);
    void scan(int scan_duration);
    bool connect_to_device();
    void trigger();

    bool is_ready_to_connect();
    BLEAddress get_device_address();
};

#endif