#include "M5StickCPlus.h"
#include "CanonBLE.h"
#include "Display.h"

String name_remote = "BR-M5";
Display M5_display = Display(&M5.Lcd, name_remote);
CanonBLE canon_ble = CanonBLE(name_remote);

void setup()
{
    Serial.begin(115200);

    M5.begin();
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
    M5_display.set_main_menu_screen();
}

void loop()
{
    M5.update();
    if (M5.BtnA.wasPressed())
    {
        canon_ble.trigger();
    }
    delay(10);
}