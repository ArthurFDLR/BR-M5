#include "Button.h"

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
