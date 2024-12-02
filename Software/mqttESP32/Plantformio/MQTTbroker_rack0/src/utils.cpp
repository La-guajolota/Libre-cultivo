/*
 * utils.cpp
 * Created on: Nov 16, 2024
 * Author: Adrián Silva Palafox
 * Description: Implementation of utility classes for embedded MQTT server functionality.
 */

#include "utils.h"

/* =======================
 * Topics Definitions
 * =======================
 * Arrays to hold MQTT topics for sensors and actuators.
 * Each element represents a specific topic within the Rack0 system.
 */
const std::string sensor_topics[] = {
    "rack0/sens/water/temperature",
    "rack0/sens/ambient/temperature",
    "rack0/sens/ambient/humidity",
    "rack0/sens/water/ph",
    "rack0/sens/water/tds",
    "rack0/sens/water/ec"};

const std::string actuator_topics[] = {
    "rack0/actu/watering0",
    "rack0/actu/dose_pump0",
    "rack0/actu/dose_pump1",
    "rack0/actu/dose_pump2",
    "rack0/actu/light/control",
    "rack0/actu/fan/control0",
    "rack0/actu/fan/control1",
    "rack0/actu/humidifier"};

/* =======================
 * Static IP Configuration
 * =======================
 * Predefined network settings for the MQTT server device.
 */
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

/* =======================
 * Function Implementations
 * =======================
 */

/**
 * Retrieves the topic string for a given sensor enum value.
 * @param sensor The sensor enum value.
 * @return The corresponding topic string or an empty string if out of bounds.
 */
std::string get_sensor_topic(SensorTopic sensor)
{
    int index = static_cast<int>(sensor);
    if (index >= 0 && index < static_cast<int>(SensorTopic::SENSOR_COUNT))
    {
        return sensor_topics[index];
    }
    return "";
}

/**
 * Retrieves the topic string for a given actuator enum value.
 * @param actuator The actuator enum value.
 * @return The corresponding topic string or an empty string if out of bounds.
 */
std::string get_actuator_topic(ActuatorTopic actuator)
{
    int index = static_cast<int>(actuator);
    if (index >= 0 && index < static_cast<int>(ActuatorTopic::ACTUATOR_COUNT))
    {
        return actuator_topics[index];
    }
    return "";
}

/**
 * Handles incoming messages for actuator topics.
 * Sends the payload to the assigned actuator via UART.
 * @param topic The topic string received.
 * @param payload The payload string associated with the topic.
 */
void handleActuatorTopic(const char *topic, const char *payload)
{
    for (int index = 0; index < static_cast<int>(ActuatorTopic::ACTUATOR_COUNT); index++)
    {
        if (topic == actuator_topics[index])
        {
            String formattedMessage = String(index) + "*" + String(payload);
            Serial1.println(formattedMessage); // Send via UART1
            Serial.println("ActuadorID:" + formattedMessage);
        }
    }
}

/**
 * Handles incoming messages for sensor topics.
 * Sends the payload to the assigned sensor via UART.
 * @param topic The topic string received.
 * @param payload The payload string associated with the topic.
 */
void handleSensorTopic(const char *topic, const char *payload)
{
    for (int index = 0; index < static_cast<int>(SensorTopic::SENSOR_COUNT); index++)
    {
        if (topic == sensor_topics[index])
        {
            String formattedMessage = String(index) + "*" + String(payload);
            Serial2.println(formattedMessage); // Send via UART2
            Serial.println("SensorID:" + formattedMessage);
        }
    }
}

/**
 * Subscribes to all predefined MQTT topics for sensors and actuators.
 * Uses lambdas to handle incoming messages for each topic.
 */
void MyMQTT::subscribeToTopics()
{
    for (int i = 0; i < static_cast<int>(SensorTopic::SENSOR_COUNT); i++)
    {
        subscribe(sensor_topics[i].c_str(), [](const char *topic, const char *payload)
                  { handleSensorTopic(topic, payload); });
    }

    for (int i = 0; i < static_cast<int>(ActuatorTopic::ACTUATOR_COUNT); i++)
    {
        subscribe(actuator_topics[i].c_str(), [](const char *topic, const char *payload)
                  { handleActuatorTopic(topic, payload); });
    }

    Serial.println("Subscribed to all Rack0 topics.");
}

/**
 * Adds a custom topic to the subscription list.
 * @param topic The custom topic string to subscribe to.
 */
void MyMQTT::addTopicToSubscription(const char *topic)
{
    subscribe(topic);
    Serial.printf("Subscribed to topic: %s\n", topic);
}

/**
 * Authenticates MQTT clients based on username and password.
 * @param client_id The client identifier string.
 * @param username The username string.
 * @param password The password string.
 * @return The connection return code indicating authentication status.
 */
PicoMQTT::ConnectReturnCode MyMQTT::auth(const char *client_id, const char *username, const char *password)
{
    if (!client_id || String(client_id).length() < 3)
    {
        return PicoMQTT::CRC_IDENTIFIER_REJECTED;
    }

    if (!username || !password)
    {
        return PicoMQTT::CRC_NOT_AUTHORIZED;
    }

    if (
        (String(username) == "adrian" && String(password) == "librecultivo") ||
        (String(username) == "juan" && String(password) == "pepexd") ||
        (String(username) == "cel" && String(password) == "celxd"))
    {
        return PicoMQTT::CRC_ACCEPTED;
    }

    return PicoMQTT::CRC_BAD_USERNAME_OR_PASSWORD;
}

/**
 * Handles UART-based MQTT message forwarding.
 * Parses the incoming message for a topic and payload, and publishes it to the broker.
 * @param serialPort The hardware serial port instance.
 * @param broker The MQTT broker instance.
 */
void MyMQTT::UART_MQTT(HardwareSerial &serialPort, MyMQTT &broker)
{
    String message = serialPort.readString();

    int dashIndex = message.indexOf('*');
    if (dashIndex != -1)
    {
        String topicPart = message.substring(0, dashIndex);
        String valuePart = message.substring(dashIndex + 1);

        Serial.println("Topic: " + topicPart);
        Serial.println("Value: " + valuePart);

        broker.publish(topicPart, valuePart);
    }
}
