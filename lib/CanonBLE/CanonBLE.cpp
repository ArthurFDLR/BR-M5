#include "CanonBLE.h"
#include <Arduino.h>
#include <BLEDevice.h>


advdCallback::advdCallback(BLEUUID service_uuid, bool* ready_to_connect, BLEAddress* address_to_connect)
{
    service_uuid_wanted = service_uuid;
    pready_to_connect = ready_to_connect;
    paddress_to_connect = address_to_connect;
}

void advdCallback::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID())
    { //Check if device has service UUID available.
        if (service_uuid_wanted.equals(advertisedDevice.getServiceUUID()))
        { //Check if it is Canon service UUID.
            *paddress_to_connect = advertisedDevice.getAddress();
            *pready_to_connect = true;
            advertisedDevice.getScan()->stop();
        }
    }
}

CanonBLE::CanonBLE(String name) :
    SERVICE_UUID("00050000-0000-1000-0000-d8492fffa821"),
    PAIRING_SERVICE("00050002-0000-1000-0000-d8492fffa821"),
    SHUTTER_CONTROL_SERVICE("00050003-0000-1000-0000-d8492fffa821")
{
    device_name = name;
}

// Purpose : Scanning for new BLE devices around.
//           When found -> advdCallback::OnResult
void CanonBLE::scan(int scan_duration)
{
    Serial.println("Start BLE scan");
    
    BLEDevice::init(device_name.c_str());
    BLEScan* pBLEScan = BLEDevice::getScan();
    advdCallback* advert_dev_callback = new advdCallback(SERVICE_UUID, &ready_to_connect, &ext_device_address);

    pBLEScan->setAdvertisedDeviceCallbacks(advert_dev_callback); // Retrieve a Scanner and set the callback we want to use to be informed when we have detected a new device.
    pBLEScan->setActiveScan(true);
    pBLEScan->start(scan_duration); // Specify that we want active scanning and start the scan to run for 30 seconds.
}

bool CanonBLE::is_ready_to_connect()
{
    return ready_to_connect;
}

BLEAddress CanonBLE::get_device_address()
{
    return ext_device_address;
}

// Purpose : Find reference to necessary characteristics and connect to M50.
bool CanonBLE::connect_to_device() {
    // Connect to external device
    if (pclient->connect(ext_device_address))
    {
        // Acquire reference to main service
        pRemoteService = pclient->getService(SERVICE_UUID);
        if (pRemoteService != nullptr)
        {
            // Acquire reference to BLE characteristics
            pRemoteCharacteristic_Pairing = pRemoteService->getCharacteristic(PAIRING_SERVICE);
            pRemoteCharacteristic_Trigger = pRemoteService->getCharacteristic(SHUTTER_CONTROL_SERVICE);
            if ((pRemoteCharacteristic_Pairing != nullptr) && (pRemoteCharacteristic_Pairing != nullptr))
            {
                // Send request on pairing service from external device
                String device_name_ = " " + device_name + " "; //Pairing message to send
                byte cmdPress[device_name_.length()]; // Stocking list of Bytes char of the message
                device_name_.getBytes(cmdPress, device_name_.length()); // message Parser
                cmdPress[0] = {0x03};
                pRemoteCharacteristic_Pairing->writeValue(cmdPress, sizeof(cmdPress), false); // Writing to Canon_pairing_service
                delay(500);
                return true;
            }
        }
    }
    return false;
}

void CanonBLE::trigger(){
    byte cmdByte[] = {MODE_IMMEDIATE|BUTTON_RELEASE}; // Binary OR : Concatenate Mode and Button
    pRemoteCharacteristic_Trigger->writeValue(cmdByte, sizeof(cmdByte), false);   // Set the characteristic's value to be the array of bytes that is actually a string.
    delay(600);
    //byte cmdrstByte[] = {BUTTON_RELEASE};
    pRemoteCharacteristic_Trigger->writeValue(MODE_IMMEDIATE, sizeof(MODE_IMMEDIATE));
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