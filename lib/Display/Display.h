#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "M5StickCPlus.h"
#include <Arduino.h>

extern const unsigned char icon_ble[4608];

class Display
{
private:
    TFT_eSprite buffer;
    String name;
    String address;
    bool negatif = true; //White text on black bg if true

    const GFXfont* font_name = &Yellowtail_32;
    const GFXfont* font_titles = &FreeSerif24pt7b;
    const GFXfont* font_text = &FreeSerif12pt7b;
    const GFXfont* font_text_small = &FreeSerif9pt7b;
public :
    Display(M5Display* tft, String name_remote);
    void set_address(String ext_address);
    void set_init_screen();
    void set_main_menu_screen(int delay, String status);
};

#endif