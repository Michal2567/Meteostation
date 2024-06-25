#include "mqtt_handler.h"

// definition of static variables
std::string mqtt_handler::server_ip = "";
std::pair<std::string, std::string> mqtt_handler::login = {"", ""};
std::string mqtt_handler::topic_global = "";
std::string mqtt_handler::client_id = "";
WiFiClient mqtt_handler::espClient;
PubSubClient mqtt_handler::client(espClient);

mqtt_handler::mqtt_handler()
{
}

mqtt_handler::~mqtt_handler()
{
}

void mqtt_handler::set(std::string ID, std::string IP, std::string TOPIC_G, std::string TOPIC_S, std::string USERNAME, std::string PASSWORD, std::function<std::string()> funct)
{
    client_id = ID;
    server_ip = IP;
    topic_global = TOPIC_G;
    topic_specific = TOPIC_S;
    login = std::make_pair(USERNAME, PASSWORD);
    function = funct;
}

void mqtt_handler::send()
{
    // check if MQTT server is connected. If disconnected, start reconnect process
    if (!client.connected())
    {
        reconnect();
    }
    // creation of payload string
    std::stringstream payload;
    payload << "{\"" << topic_specific << "\": " << function() << "}";

    // publishing data
    client.publish((topic_global + topic_specific + "/state").c_str(), payload.str().c_str());
}

// starting MQTT
void mqtt_handler::begin()
{
    client.setServer(server_ip.c_str(), 1883);
    reconnect();
}

// reconnect process
void mqtt_handler::reconnect()
{
    int attempts = 0;
    while (true)
    {
        // if device tries to connect more than 10 times without success, it will restart
        if (attempts > 10)
            esp_restart();

        std::cout << "Attempting MQTT connection..." << std::endl;
        // Attempt to connect
        if (client.connect(client_id.c_str(), login.first.c_str(), login.second.c_str()))
        {
            // connection successful. Break the loop and send a message about the connection
            std::cout << "Connected" << std::endl;
            client.publish((topic_global + topic_specific).c_str(), (client_id + topic_specific + " connected").c_str());
            break;
        }
        else
        {
            // connection failed. Wait 5 seconds and try again.
            std::cout << "Failed, rc=" << client.state() << " try again in 5 seconds" << std::endl;
            // Wait 5 seconds before retrying
            delay(5000);
        }
        attempts++;
    }
}