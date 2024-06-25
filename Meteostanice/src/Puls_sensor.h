#ifndef PULS_SENSOR_H
#define PULS_SENSOR_H

#include <Arduino.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

class Puls_sensor
{
public:
    // (int pin number, double correction constant, int time from last pulse to zero, bool 1=pullup 0=pulldown, int number of stored samples)
    Puls_sensor(int pin, double conversion, int deadtime, bool pull, int length);
    // (int pin number, double correction constant, int time from last pulse to zero, int number of stored samples)
    Puls_sensor(int pin, double pulse_to_time, int deadtime, int length);
    // default destructor
    ~Puls_sensor();
    // initialize sensor
    void begin();
    // return string with value
    std::string get();

private:
    // number of samples
    int vector_length;
    // pin of sensor
    int pin;
    // constant to convert raw value to correct result
    double conversion;
    // time from last pulse to zero
    int deadTime;
    // specify between pullup and pulldown
    bool pullup;
    // vector holding raw values
    std::vector<long> cas;
    // function called at interrupt
    void newTime();

    // at interrupt event decide which object will be called (void * pointer to general object), IRAM_ATTR sign on RAM in CPU core
    static void IRAM_ATTR interruptHandler(void *arg);
};

#endif // PULS_SENSOR_H