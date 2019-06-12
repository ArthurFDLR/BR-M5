#ifndef Button_h
#define Button_h

#include <Arduino.h>


class Button {
  public :
  
    Button(int pin);    
    bool Detect_click();

  private :
    int _pinButton;
    int _stateLED = LOW;
    int _stateButton;
    int _previous = LOW;
    long _timeButton = 0;
    long _debounce = 300;
};
#endif
