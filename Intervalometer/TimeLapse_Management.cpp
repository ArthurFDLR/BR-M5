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


TimeLapse::TimeLapse(int inter,void(*trigger)())
{
  TimeLapse_ON = false;
  _Interval = inter;
  _func_trigger = trigger;
}

void TimeLapse::TimeLapse_Trigger()
{
  if (TimeLapse_ON and (millis() - _time_last_trigger) > _Interval){
    _func_trigger();
    _time_last_trigger = millis();
  }
}
