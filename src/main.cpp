#include "CanonBLERemote.h"
#include <Arduino.h>
#include "Ticker.h"
#include "M5StickCPlus.h"
#include "Display.h"
#include "TimeLapse_Management.h"

#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"
#include <esp32-hal-log.h>

String name_remote = "BR-M5";
CanonBLERemote canon_ble(name_remote);
Display M5_display(&M5.Lcd, name_remote);
String status = "init";

TimeLapse timelapse(400);

enum RemoteMode {Settings, Shooting}current_mode;

void setup()
{
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_INFO); 

    M5.begin();
    bool do_pair = M5.BtnA.isPressed();
    M5.Axp.ScreenBreath(9);
    M5.Lcd.setRotation(1);
    M5_display.set_init_screen(do_pair);
    

    current_mode = Shooting;

    canon_ble.init();
    
    delay(500);
    // Pairing
    if(do_pair){
        // pair() function should be called only when you want to pair with the new camera. 
        // After paired, the pair() function should not be called.
        do{
            Serial.println("Pairing...");
        }
        while(!canon_ble.pair(10));

    }

    delay(500);
    Serial.println("Setup Done");
    
    M5_display.set_address(canon_ble.getPairedAddressString());
    status = "Ready for single shot";
    M5_display.set_main_menu_screen(timelapse.get_interval(), status);
}


void update_shooting()
{
    // Update timelapse
    if (timelapse.TimeLapse_Trigger())
    {
        Serial.println("Trigger timelapse");
        if(!canon_ble.trigger()){
            Serial.println("Trigger Failed");
        }
    }

    // Remote control
    if (timelapse.get_interval() == 0) //Single shots
    {
        if (M5.BtnA.wasReleased() && !M5.BtnB.wasReleased())
        {
            Serial.println("Single shot");
            if(!canon_ble.trigger()){
                Serial.println("Trigger Failed");
            }
        }
    }
    else // Timelapses
    {
        // Stop or start timelapse
        if (M5.BtnA.wasReleased())
        {
            if (timelapse.Recording_OnOFF())
            {
                Serial.println("Start timelapse");
                status = "Shooting timelapse";
                M5_display.set_main_menu_screen(timelapse.get_interval(), status);
            }
            else
            {
                Serial.println("Stop timelapse");
                status = "Ready for timelapse";
                M5_display.set_main_menu_screen(timelapse.get_interval(), status);
            }
        }
    }
}

void update_settings()
{
    if (M5.BtnA.wasReleased())
    {
        timelapse.TimeLapse_decDelay();
        status = "Setting interval";
        M5_display.set_main_menu_screen(timelapse.get_interval(), status);
    }
    if (M5.BtnB.wasReleased())
    {
        timelapse.TimeLapse_incDelay();
        status = "Setting interval";
        M5_display.set_main_menu_screen(timelapse.get_interval(), status);
    }
}

void loop()
{
    // Update buttons state
    M5.update();

    switch (current_mode)
    {
    case Settings:
        if (M5.BtnB.wasReleasefor(700))
        {
            // M5.BtnB.reset();
            current_mode = Shooting;
            status = (timelapse.get_interval()==0)?"Ready for single shot":"Ready for timelapse";
        }
        else
        {
            update_settings();
        }
        break;
    
    case Shooting:
        if (M5.BtnB.wasReleasefor(700))
        {
            // M5.BtnB.reset();
            current_mode = Settings;
            status = "Setting interval";
        }
        else
        {
            update_shooting();
        }
        break;

    default:
        break;
    }

    M5_display.set_main_menu_screen(timelapse.get_interval(), status);
    delay(10);
}