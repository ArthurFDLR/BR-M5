#include "M5StickCPlus.h"
#include "CanonBLE.h"
#include "Display.h"
#include "TimeLapse_Management.h"

String name_remote = "BR-M5";
Display M5_display(&M5.Lcd, name_remote);
CanonBLE canon_ble(name_remote);
TimeLapse timelapse(400);

enum RemoteMode {Settings, Shooting}current_mode;

void setup()
{
    Serial.begin(115200);
    M5.begin();
    current_mode = Shooting;
    M5.Axp.ScreenBreath(9);
    M5.Lcd.setRotation(1);
    M5_display.set_init_screen();

    /*
    while (! canon_ble.is_ready_to_connect())
    {
        canon_ble.scan(5);
    }
    Serial.print("Canon device found: ");
    Serial.println(canon_ble.get_device_address().toString().c_str());
    
    if (canon_ble.connect_to_device())
    {
        M5_display.set_address(canon_ble.get_device_address().toString().c_str());
        M5_display.set_main_menu_screen();
        Serial.println("Connected successfully");
    }
    */
    M5_display.set_address("00:9d:6b:4c:01:bd");
    M5_display.set_main_menu_screen(timelapse.get_interval(), "Ready for single shot");
}

void update_shooting()
{
    // Update timelapse
    if (timelapse.TimeLapse_Trigger())
    {
        //canon_ble.trigger();
        Serial.println("Trigger tl");
    }

    // Remote control
    if (timelapse.get_interval() == 0) //Single shots
    {
        if (M5.BtnA.wasReleased() && !M5.BtnB.wasReleased())
        {
            //canon_ble.trigger();
            Serial.println("Single shot");
        }
    }
    else // Timelapses
    {
        // Stop or start timelapse
        if (M5.BtnA.wasReleased() && !M5.BtnB.wasReleased())
        {
            if (timelapse.Recording_OnOFF())
            {
                Serial.println("Start timelapse");
                M5_display.set_main_menu_screen(timelapse.get_interval(), "Shooting timelapse");
            }
            else
            {
                Serial.println("Stop timelapse");
                M5_display.set_main_menu_screen(timelapse.get_interval(), "Ready for timelapse");
            }
        }
    }
}

void update_settings()
{
    if (M5.BtnA.wasReleased() && !M5.BtnB.wasReleased())
    {
        timelapse.TimeLapse_decDelay();
        M5_display.set_main_menu_screen(timelapse.get_interval(), "Setting interval");
    }
    if (!M5.BtnA.wasReleased() && M5.BtnB.wasReleased())
    {
        timelapse.TimeLapse_incDelay();
        M5_display.set_main_menu_screen(timelapse.get_interval(), "Setting interval");
    }
}

void loop()
{
    // Update buttons state
    M5.update();

    switch (current_mode)
    {
    case Settings:
        if (M5.BtnB.pressedFor(700))
        {
            M5.BtnB.reset();
            current_mode = Shooting;
            String status = (timelapse.get_interval()==0)?"Ready for single shot":"Ready for timelapse";
            M5_display.set_main_menu_screen(timelapse.get_interval(), status);
        }
        else
        {
            update_settings();
        }
        break;
    
    case Shooting:
        if (M5.BtnB.pressedFor(700))
        {
            M5.BtnB.reset();
            current_mode = Settings;
            M5_display.set_main_menu_screen(timelapse.get_interval(), "Setting interval");
        }
        else
        {
            update_shooting();
        }
        break;

    default:
        break;
    }

    // Delay setting
    delay(10);
}