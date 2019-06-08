/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef BLE_Manager_M50_h
#define BLE_Manager_M50_h

#include <Arduino.h>

class BLE_M50
{
  public:
    BLE_M50(int pin);
    void dot();
    void dash();
  private:
    int _pin;
};

#endif
