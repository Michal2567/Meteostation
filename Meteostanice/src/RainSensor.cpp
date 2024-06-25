#include "RainSensor.h"

RainSensor::RainSensor(int pin, double puls_to_time) : Puls_sensor(pin, puls_to_time, 600, 1, 4) {}
RainSensor::RainSensor(int pin, double puls_to_time, int deadTime) : Puls_sensor(pin, puls_to_time, deadTime, 1, 4) {}

RainSensor::~RainSensor()
{
}