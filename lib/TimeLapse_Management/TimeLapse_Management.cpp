#include "TimeLapse_Management.h"

TimeLapse::TimeLapse(long inter_min)
{
    timeLapse_ON = false;
    _MIN_Interval = inter_min;
}

bool TimeLapse::TimeLapse_Trigger()
{
    if (timeLapse_ON and ((millis() - _time_last_trigger) > Interval)){
        Pic_count += 1;
        _time_last_trigger = millis();
        return true;
    }
    else{
        return false;
    }
}

void TimeLapse::TimeLapse_incDelay()
{
    if (Interval < _MIN_Interval)
    {
        Interval = _MIN_Interval;
    }
    else
    {
        Interval += _delay_increment;
    }
}

void TimeLapse::TimeLapse_decDelay()
{
    if (Interval - _delay_increment < _MIN_Interval)
    {
        Interval = 0;
    }
    else
    {
        Interval -= _delay_increment;
    }
}

void TimeLapse::Launch()
{
    if (Interval >= _MIN_Interval)
    {
        timeLapse_ON = true;
        Pic_count = 0;
    }
}

void TimeLapse::Stop()
{
    timeLapse_ON = false;
}

bool TimeLapse::Recording_OnOFF()
{
    if (timeLapse_ON)
    {
        Stop();
        return false;
    }
    else
    {
        Launch();
        return true;
    }
}

long TimeLapse::get_interval()
{
    return Interval;
}