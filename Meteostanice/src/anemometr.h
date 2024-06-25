#ifndef ANEMOMETR_H
#define ANEMOMETR_H

#include "Puls_sensor.h"

class anemometr : public Puls_sensor
{
public:
    // (int pin number, double correction constant)
    anemometr(int pin, double time_to_speed);
    // (int pin number, double correction constant, int time from last pulse to zero)
    anemometr(int pin, double time_to_speed, int deadTime);
    ~anemometr();

private:
};

#endif // ANEMOMETR_H