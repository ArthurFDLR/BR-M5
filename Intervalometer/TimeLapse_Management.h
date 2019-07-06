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
    //constructor
    //IN/ inter_min : lower delay between shots (in ms): requiered time to reconnect to the cam
    //IN/ trigger : function taking shot
    TimeLapse(long inter_min,void(*trigger)());
    //trigger each #Interval ms
    void TimeLapse_Trigger();
    //increase #Interval
    void TimeLapse_incDelay();
    //decrease #Interval (> #_MIN_Interval)
    void TimeLapse_decDelay();
    
    bool TimeLapse_ON;
    long Interval = 4000; //ms

  private :
    void(*_func_trigger)();
    long _time_last_trigger = 0; //ms
    long _MIN_Interval; //ms
    const long _delay_increment = 100; //ms
};

#endif
