#include "TimeLapse_Management.h"

Button::Button(int pin)
{
  _pinButton = pin;
}

// Debounced timing -> _debounced
bool Button::Detect_click()
{
  bool result = false;
  _stateButton = digitalRead(_pinButton);  // Quand on appuis sur le boutton ...
  if(_stateButton == HIGH && _previous == LOW && millis() - _timeButton > _debounce) {
    _timeButton = millis();
    result = true;
  }
  _previous == _stateButton;
  return result;
}


TimeLapse::TimeLapse(long inter_min,void(*trigger)())
{
  TimeLapse_ON = false;
  _MIN_Interval = inter_min;
  _func_trigger = trigger;
}

void TimeLapse::TimeLapse_Trigger()
{
  if (TimeLapse_ON and ((millis() - _time_last_trigger) > Interval)){
    _time_last_trigger = millis();
    _func_trigger();
  }
}

void TimeLapse::TimeLapse_incDelay()
{
  Interval += _delay_increment;
}

void TimeLapse::TimeLapse_decDelay()
{
  if (Interval - _delay_increment >= _MIN_Interval){
    Interval -= _delay_increment;
  }
}
