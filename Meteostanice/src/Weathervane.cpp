#include "Weathervane.h"

direction::direction(std::string DIR = "0", double value = 0) : voltage(value), dir(DIR) {}

Weathervane::Weathervane(int pin, std::vector<direction> &dir) : pin(pin), directions(dir) {}

Weathervane::~Weathervane() {}

void Weathervane::begin()
{
    std::cout << "Weathervane begin" << std::endl;
}

std::string Weathervane::get()
{
    double value = analogReadMilliVolts(pin); // read voltage level from pin in millivolts
    direction out;
    double min = std::numeric_limits<double>::max();

    // find nearest voltage level from preset to measured
    for (auto &dir : directions)
    {
        double tmp = std::abs(value - dir.voltage);
        if (tmp < min)
        {
            out = dir;
            min = tmp;
        }
    }
    // return corresponding angle
    return out.dir;
}