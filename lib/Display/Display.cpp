#include "Display.h"
#include <Arduino.h>
#include "M5StickCPlus.h"

Display::Display(M5Display* tft, String name_remote):
    buffer(tft),
    name(name_remote)
{
    buffer.createSprite(240, 135);
    buffer.setTextColor((negatif?TFT_WHITE:TFT_BLACK));
    buffer.setCursor(0, 0);
}

void Display::set_init_screen()
{
    buffer.fillRect(0, 0, 240, 135, (negatif?TFT_BLACK:TFT_WHITE));
    
    buffer.setTextSize(2);
    buffer.setFreeFont(font_name);
    buffer.drawString(name, 120 - (buffer.textWidth(name)/2.0), 20 +  font_name->glyph->height); 
    
    buffer.setTextSize(1);
    buffer.setFreeFont(font_text);
    buffer.drawString("Pairing in progress", 120 - (buffer.textWidth("Pairing in progress")/2.0), 95); 

    //display_buffer.setTextSize(2);
    //display_buffer.setCursor(10, 70);
    //display_buffer.printf("Waiting for connection");
    //buffer.pushImage(180, 16, 48, 48,  (uint16_t *)icon_ble);
    
    buffer.pushSprite(0,0);
}

void Display::set_address(String address_str)
{
    address = address_str;
    address.toUpperCase();
}

void Display::set_main_menu_screen()
{
    buffer.fillRect(0, 0, 240, 135, (negatif?TFT_BLACK:TFT_WHITE));

    buffer.setFreeFont(font_text);
    buffer.setTextSize(1);
    buffer.drawString(address, 10, 10);

    buffer.pushSprite(0,0);
}