// include local files
#include "anemometr.h"
#include "RainSensor.h"
#include "Weathervane.h"
#include "mqtt_handler.h"
// include necessary libraries
#include <Arduino.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <WiFi.h>

// pins where sensors are connected
#define ANEMOMETR_PIN 13
#define RAIN_SENSOR_PIN 27
#define WEATHERVANE_PIN 34

#define CLIENT_ID "meteostation 1" // ID of MQTT client
#define SSID "Network_Hoskova"     // SSID of WiFi network
#define PASSWORD "hosk1234"        // password for WiFi
#define IP "192.168.1.27"          // IP of server
#define USERNAME_MQTT "mott"       // username for MQTT server
#define PASSWORD_MQTT "1234"       // password for MQTT server
#define BASE_TOPIC "meteostation_1/" // base topic for all messages published from the device

// vector of wind directions and corresponding voltage levels in mV
std::vector<direction> dir = {
    direction("0", 3060),
    direction("22.5", 2700),
    direction("45", 2880),
    direction("67.5", 2310),
    direction("90", 2570),
    direction("112.5", 1400),
    direction("135", 1520),
    direction("157.5", 290),
    direction("180", 330),
    direction("202.5", 240),
    direction("225", 620),
    direction("247.5", 430),
    direction("270", 960),
    direction("292.5", 810),
    direction("315", 2070),
    direction("337.5", 1970),
};

// creation of sensor instances
Weathervane vane(WEATHERVANE_PIN, dir);
anemometr wind(ANEMOMETR_PIN, 2.4, 10);
RainSensor rain(RAIN_SENSOR_PIN, 1005.84);
// vector stores MQTT instances
std::vector<mqtt_handler> mqtts;

void wifi_error() // stops program until WiFi is connected
{
  while (WiFi.status() != WL_CONNECTED)
  {
    std::cout << "WiFi not connected" << std::endl;
    delay(1000);
  }
}

mqtt_handler mqtt_setup(std::string topic_specific, std::function<std::string()> funct) // create and set up MQTT instance and return it
{
  mqtt_handler tmp;
  tmp.set(CLIENT_ID, IP, BASE_TOPIC, topic_specific, USERNAME_MQTT, PASSWORD_MQTT, funct);
  return tmp;
}

void setup()
{
  // sensor initialization
  wind.begin();
  rain.begin();
  vane.begin();
  // WiFi initialization
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  if (WiFi.status() != WL_CONNECTED)
  {
    wifi_error();
  }
  std::cout << "WiFi connected" << std::endl;
  // creation of MQTT instances and adding them to the vector
  mqtts.push_back(mqtt_setup("wind_speed", std::bind(&Puls_sensor::get, &wind)));
  mqtts.push_back(mqtt_setup("rain_fall", std::bind(&Puls_sensor::get, &rain)));
  mqtts.push_back(mqtt_setup("wind_direction", std::bind(&Weathervane::get, &vane)));
  // begin all MQTT instances
  for (mqtt_handler &a : mqtts)
  {
    a.begin();
  }
}

void loop()
{
  for (mqtt_handler &a : mqtts) // run through all instances and send data to server
  {
    a.send();
  }

  delay(60000); // wait 60 seconds and send data again
}
