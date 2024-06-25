#include "anemometr.h"
#include <Arduino.h>
#include <vector>

anemometr::anemometr(int pin, double time_to_speed) : Puls_sensor(pin, time_to_speed, 120, 1,100) {}
anemometr::anemometr(int pin, double time_to_speed, int deadTime) : Puls_sensor(pin, time_to_speed, deadTime, 1,100) {}

anemometr::~anemometr()
{
}
