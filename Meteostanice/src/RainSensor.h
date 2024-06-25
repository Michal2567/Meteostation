#ifndef RAINSENSOR_H
#define RAINSENSOR_H

#include "Puls_sensor.h"
#include <vector>

void preruseni();

class RainSensor : public Puls_sensor
{
public:
    // (int pin number, double correction constant)
    RainSensor(int pin, double pulse_to_time);
    // (int pin number, double correction constant, int time from last pulse to zero)
    RainSensor(int pin, double pulse_to_time, int deadTime);
    ~RainSensor();
    void send();

private:
};

#endif // RAINSENSOR_H