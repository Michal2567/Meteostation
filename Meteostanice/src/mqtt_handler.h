#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <iostream>
#include <functional>
#include <sstream>
#include <WiFi.h>
#include <PubSubClient.h>

class mqtt_handler
{
public:
    // default constructor
    mqtt_handler();
    // default destructor
    ~mqtt_handler();
    // function to set up variables in instance
    // (std::string ID of MQTT client, std::string IP address of server, std::string topic part common for all instances, std::string topic specific to instance, std::string username for server, std::string password for server, std::function<std::string()> function returning std::string data that will be published)
    void set(std::string ID, std::string IP, std::string TOPIC_G, std::string TOPIC_S, std::string USERNAME, std::string PASSWORD, std::function<std::string()> funct);
    // send data returned by function
    void send();
    // initialize communication with server
    void begin();

private:
    // Store IP of server. Same for all instances.
    static std::string server_ip;
    // specific part of topic. Only for this instance
    std::string topic_specific;
    // Part of topic for all instances
    static std::string topic_global;
    // ID of MQTT client
    static std::string client_id;
    // Login data for server. Same for all instances.
    static std::pair<std::string, std::string> login;
    // function returning published data
    std::function<std::string()> function;
    // when communication with server is stopped, stops program until communication is reinitialized
    void reconnect();
    // Store actual WiFi data. Same for all instances.
    static WiFiClient espClient;
    // part of PubSubClient library. Enables interaction with library. Same for all instances.
    static PubSubClient client;
};

#endif // MQTT_HANDLER_H