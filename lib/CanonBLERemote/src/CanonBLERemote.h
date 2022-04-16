#ifndef CANON_BLE_REMOTE_H_
#define CANON_BLE_REMOTE_H_

#include <BLEDevice.h>
#include <Arduino.h>
#include <ArduinoNvs.h>

class advdCallback : public BLEAdvertisedDeviceCallbacks
{
private:
    BLEAddress *paddress_to_connect;
    BLEUUID service_uuid_wanted;
    bool *pready_to_connect;

public:
    advdCallback(BLEUUID service_uuid, bool *ready_to_connect, BLEAddress *address_to_connect);
    void onResult(BLEAdvertisedDevice advertisedDevice);
};

class ConnectivityState : public BLEClientCallbacks
{
private:
    bool connected = false;

public:
    void onConnect(BLEClient *pclient) override;
    void onDisconnect(BLEClient *pclient) override;
    bool isConnected();
};

class CanonBLERemote
{
private:
    // Trigger options
    const byte BUTTON_RELEASE = 0b10000000;
    const byte BUTTON_FOCUS = 0b01000000;
    const byte BUTTON_TELE = 0b00100000;
    const byte BUTTON_WIDE = 0b00010000;
    const byte MODE_IMMEDIATE = 0b00001100;
    const byte MODE_DELAY = 0b00000100;
    const byte MODE_MOVIE = 0b00001000;

    const BLEUUID SERVICE_UUID;
    const BLEUUID PAIRING_SERVICE;
    const BLEUUID SHUTTER_CONTROL_SERVICE;

    BLEClient *pclient = BLEDevice::createClient();
    ConnectivityState *pconnection_state = new ConnectivityState();
    BLEAddress camera_address = BLEAddress("");
    BLERemoteService *pRemoteService;
    BLERemoteCharacteristic *pRemoteCharacteristic_Pairing;
    BLERemoteCharacteristic *pRemoteCharacteristic_Trigger;
    ArduinoNvs nvs;

    bool ready_to_connect = false;
    String device_name = "";
    void scan(unsigned int  scan_duration);
    bool connect();
    void disconnect();


public:
    CanonBLERemote(String name);
    void init();
    bool pair(unsigned int scan_duration);
    bool isConnected();

    bool trigger();
    bool focus();

    BLEAddress getPairedAddress();
    String getPairedAddressString();
};

#endif