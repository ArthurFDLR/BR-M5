#ifndef TimeLapse_Management_h
#define TimeLapse_Management_h

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

class TimeLapse {
  public :
    TimeLapse(int inter,void(*trigger)());
    bool TimeLapse_ON;
    void TimeLapse_Trigger();

  private :
    void(*_func_trigger)();
    long _Interval;
    long _time_last_trigger = 0;
};

#endif
