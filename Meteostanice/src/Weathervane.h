/*
This class handles the working of a weathervane, outputting discrete voltage levels to specific directions.
*/

#ifndef WEATHERVANE_H
#define WEATHERVANE_H

#include <Arduino.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

struct direction
{
    // (name connected to value, voltage value in millivolts)
    direction(std::string, double);
    // stores voltage data
    double voltage;
    // stores direction corresponding to voltage
    std::string dir;
};

class Weathervane
{
public:
    // constructor (int pin, std::vector<direction> &direction definition)
    Weathervane(int pin, std::vector<direction> &dir);
    // default destructor
    ~Weathervane();
    // initializes function of sensor
    void begin();
    // returns direction of wind corresponding to voltage
    std::string get();

private:
    // pin where sensor is connected
    int pin;
    // vector of directions
    std::vector<direction> directions;
};

#endif // WEATHERVANE_H