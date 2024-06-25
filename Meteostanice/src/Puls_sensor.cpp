#include "Puls_sensor.h"

// constructors
Puls_sensor::Puls_sensor(int pin, double conversion, int deadtime, bool pull, int length) : conversion(conversion), pin(pin), deadTime(deadtime), pullup(pull), vector_length(length) {}

Puls_sensor::Puls_sensor(int pin, double pulse_to_time, int deadtime, int length) : Puls_sensor(pin, pulse_to_time, deadtime, 0, length) {}

// destructor
Puls_sensor::~Puls_sensor()
{
    detachInterrupt(digitalPinToInterrupt(pin)); // end interrupt on instance pin
}

void Puls_sensor::newTime()
{
    // read time in microseconds
    long tmp = micros();
    // shift all data in vector to create space for new time
    for (int i = 1; i < vector_length; i++)
    {
        cas.at(i - 1) = cas.at(i);
    }
    // add new data to vector
    cas.at(vector_length - 1) = tmp;
}

std::string Puls_sensor::get()
{
    // check if the last pulse from the sensor isn't too old
    if (micros() - cas.at(vector_length - 1) > deadTime * 1000000)
    {
        // set vector cas to actual time
        cas.assign(vector_length, micros());
        // when it is too old, return zero
        return "0";
    }
    // creation of temporary variables
    double corrected;
    long time = 0;
    double PulsesPerSecond;

    // calculate average time
    for (int i = 1; i < vector_length; i++)
    {
        time += cas.at(i) - cas.at(i - 1);
    }
    time = time / (vector_length - 1);

    // when time = 0 return zero to prevent dividing by 0
    if (time == 0)
        return "0";

    // calculate output value
    PulsesPerSecond = 1000000.0 / time;
    corrected = PulsesPerSecond * conversion;
    // transform output value to string
    std::stringstream ss;
    ss << corrected;
    return ss.str();
}

// at interrupt event decide which object will be called (void * pointer to general object), IRAM_ATTR sign on RAM in CPU core
void IRAM_ATTR Puls_sensor::interruptHandler(void *arg)
{
    Puls_sensor *sensor = static_cast<Puls_sensor *>(arg); // retype general pointer to pointer on Puls_sensor instance
    sensor->newTime();                                     // call newTime in correct instance
}

void Puls_sensor::begin()
{
    cas.resize(vector_length, micros()); // resize vector cas and fill with actual time

    // set pullup/pulldown
    if (pullup == 1)
    {
        pinMode(pin, INPUT_PULLUP);
    }
    else
    {
        pinMode(pin, INPUT_PULLDOWN);
    }

    // initialize interrupt on correct pin
    attachInterruptArg(digitalPinToInterrupt(pin), interruptHandler, this, FALLING);
}