#include "CanonBLERemote.h"
#include <Arduino.h>
#include <BLEDevice.h>

static const char *LOG_TAG = "BRM5_Security_Tag";

advdCallback::advdCallback(BLEUUID service_uuid, bool *ready_to_connect, BLEAddress *address_to_connect)
{
    service_uuid_wanted = service_uuid;
    pready_to_connect = ready_to_connect;
    paddress_to_connect = address_to_connect;
}

void advdCallback::onResult(BLEAdvertisedDevice advertisedDevice)
{
    if (advertisedDevice.haveServiceUUID())
    { //Check if device has service UUID available.
        if (service_uuid_wanted.equals(advertisedDevice.getServiceUUID()))
        {
            *paddress_to_connect = advertisedDevice.getAddress();
            *pready_to_connect = true;
            advertisedDevice.getScan()->stop();
        }
    }
}

void ConnectivityState::onConnect(BLEClient *pclient)
{
    // Serial.println("Device is connected");
    connected = true;
}

void ConnectivityState::onDisconnect(BLEClient *pclient)
{
    // Serial.println("Device disconnnect");
    connected = false;
}

bool ConnectivityState::isConnected()
{
    return connected;
}

class SecurityCallback : public BLESecurityCallbacks
{

    uint32_t onPassKeyRequest()
    {
        return 123456;
    }
    void onPassKeyNotify(uint32_t pass_key)
    {
        ESP_LOGE(LOG_TAG, "The passkey Notify number:%d", pass_key);
    }
    bool onConfirmPIN(uint32_t pass_key)
    {
        ESP_LOGI(LOG_TAG, "The passkey YES/NO number:%d", pass_key);
        vTaskDelay(5000);
        return true;
    }
    bool onSecurityRequest()
    {
        ESP_LOGI(LOG_TAG, "Security Request");
        return true;
    }
    void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl)
    {
        if (auth_cmpl.success)
        {
            ESP_LOGI(LOG_TAG, "remote BD_ADDR:");
            esp_log_buffer_hex(LOG_TAG, auth_cmpl.bd_addr, sizeof(auth_cmpl.bd_addr));
            ESP_LOGI(LOG_TAG, "address type = %d", auth_cmpl.addr_type);
        }
        ESP_LOGI(LOG_TAG, "pair status = %s", auth_cmpl.success ? "success" : "fail");
    }
};

CanonBLERemote::CanonBLERemote(String name) : SERVICE_UUID("00050000-0000-1000-0000-d8492fffa821"),
                                              PAIRING_SERVICE("00050002-0000-1000-0000-d8492fffa821"),
                                              SHUTTER_CONTROL_SERVICE("00050003-0000-1000-0000-d8492fffa821")
{
    device_name = name;
    // Add our connection callback for state tracking.
    pclient->setClientCallbacks(pconnection_state);
}

void CanonBLERemote::init()
{
    BLEDevice::init(device_name.c_str());
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_NO_MITM);
    BLEDevice::setSecurityCallbacks(new SecurityCallback());

    if (nvs.begin())
    {
        log_e("Initialize NVS Success");
        String address = nvs.getString("cameraaddr");

        if (address.length() == 17)
        {
            // Serial.printf("Paired camera address: %s\n", address.c_str());
            camera_address = BLEAddress(address.c_str());
        }
        else
        {
            // Serial.println("No camera has been paired yet.");
        }
    }
    else
    {
        log_e("Initialize NVS Failed");
    }
}

// Purpose : Scanning for new BLE devices around.
//           When found -> advdCallback::OnResult
void CanonBLERemote::scan(unsigned int scan_duration)
{

    log_i("Start BLE scan");
    BLEScan *pBLEScan = BLEDevice::getScan();
    advdCallback *advert_dev_callback = new advdCallback(SERVICE_UUID, &ready_to_connect, &camera_address);

    pBLEScan->setAdvertisedDeviceCallbacks(advert_dev_callback); // Retrieve a Scanner and set the callback we want to use to be informed when we have detected a new device.
    pBLEScan->setActiveScan(true);
    pBLEScan->start(scan_duration); // Specify that we want active scanning and start the scan to run for 30 seconds.
}

BLEAddress CanonBLERemote::getPairedAddress()
{
    return camera_address;
}

String CanonBLERemote::getPairedAddressString()
{
    return String(camera_address.toString().c_str());
}

/**
 *  Scan and pair camera 
 * 
 *  This function should be called only when you want to pair with the new camera, and the camera is in remote paring screen.
 *  After paired, camera mac address will be stored in ESP32 NVS for later connection use.
 * 
 *  @param  scan_duration : Scan duration in seconds
 */

bool CanonBLERemote::pair(unsigned int scan_duration)
{
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
    log_i("Scanning for camera...");
    scan(scan_duration);
    unsigned long start_ms = millis();
    while (!ready_to_connect && millis() - start_ms < scan_duration * 1000)
    {
    }

    if (ready_to_connect)
    {
        log_i("Canon device found");
        // Serial.println(camera_address.toString().c_str());
    }
    else
    {
        log_i("Camera not found");
        return false;
    }

    // Pair camera
    log_i("Pairing..");
    if (pclient->connect(camera_address))
    {
        // Acquire reference to main service
        pRemoteService = pclient->getService(SERVICE_UUID);
        if (pRemoteService != nullptr)
        {
            // Acquire reference to BLE characteristics
            pRemoteCharacteristic_Pairing = pRemoteService->getCharacteristic(PAIRING_SERVICE);
            if ((pRemoteCharacteristic_Pairing != nullptr))
            {
                // Send request on pairing service from external device
                String device_name_ = " " + device_name + " ";          //Pairing message to send
                byte cmdPress[device_name_.length()];                   // Stocking list of Bytes char of the message
                device_name_.getBytes(cmdPress, device_name_.length()); // message Parser
                cmdPress[0] = {0x03};
                pRemoteCharacteristic_Pairing->writeValue(cmdPress, sizeof(cmdPress), false); // Writing to Canon_pairing_service
                log_e("Camera paring success");
                delay(200);
                disconnect();
                BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_NO_MITM);
                delay(200);
                connect();
                nvs.setString("cameraaddr", String(camera_address.toString().c_str()));
                if (nvs.commit())
                {
                    log_i("Saving camera's address to NVS success");
                    return true;
                }
                else
                {
                    log_e("Storing camera's address to NVS failed");
                    return false;
                }
            }
            else
            {
                log_e("Couldn't acquire the pairing or shutter service");
            }
        }
        else
        {
            log_e("Couldn't acquire the remote main service");
        }
    }
    else
    {
        log_e("Couldn't connect the BLEClient to the device");
    }
    return false;
}

bool CanonBLERemote::connect()
{
    if (pclient->connect(camera_address))
    {
        pRemoteService = pclient->getService(SERVICE_UUID);
        if (pRemoteService != nullptr)
        {
            // Serial.println("Get remote service OK");
            pRemoteCharacteristic_Trigger = pRemoteService->getCharacteristic(SHUTTER_CONTROL_SERVICE);
            if (pRemoteCharacteristic_Trigger != nullptr)
            {
                log_i("Camera connection Success");
                // disconnect();       // Disconnect remote from the camera every time after action, as the real canon remote did.
                return true;
            }
            else
            {
                log_e("Get trigger service failed");
            }
        }
        else
        {
            log_e("Couldn't acquire the remote main service");
        }
        disconnect();
    }
    return false;
}

void CanonBLERemote::disconnect()
{
    pclient->disconnect();
}

bool CanonBLERemote::isConnected()
{
    return pconnection_state->isConnected();
}

/** Trigger Camera 
 *  If the camera is in photo mode, it will take a single picture.
 *  If the camera is in movie mode, it will start/stop movie recording. 
 */

bool CanonBLERemote::trigger()
{

    if (!isConnected())
    {
        if (!connect())
        {
            return false;
        }
    }

    byte cmdByte = {MODE_IMMEDIATE | BUTTON_RELEASE};          // Binary OR : Concatenate Mode and Button
    pRemoteCharacteristic_Trigger->writeValue(cmdByte, false); // Set the characteristic's value to be the array of bytes that is actually a string.
    delay(200);
    pRemoteCharacteristic_Trigger->writeValue(MODE_IMMEDIATE, false);
    delay(50);
    return true;
}

bool CanonBLERemote::focus()
{
    if (!isConnected())
    {
        if (!connect())
        {
            return false;
        }
    }
    byte cmdByte[] = {MODE_IMMEDIATE | BUTTON_FOCUS};                    // Binary OR : Concatenate Mode and Button
    pRemoteCharacteristic_Trigger->writeValue(cmdByte, sizeof(cmdByte)); // Set the characteristic's value to be the array of bytes that is actually a string.
    delay(200);
    pRemoteCharacteristic_Trigger->writeValue(MODE_IMMEDIATE, sizeof(MODE_IMMEDIATE));
    return true;
}
