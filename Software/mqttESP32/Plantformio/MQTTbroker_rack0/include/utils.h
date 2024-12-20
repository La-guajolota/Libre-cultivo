/*
 * utils.h
 * Created on: Nov 16, 2024
 * Author: Adrián Silva Palafox
 * Description: Utility classes and configurations for embedded MQTT server functionality.
 */

#ifndef UTILS_H_
#define UTILS_H_

/* =======================
 * Libraries
 * =======================
 * Required headers for MQTT server, Arduino framework,
 * and IP address management.
 */
#include "PicoMQTT.h"  // Lightweight MQTT server library
#include <Arduino.h>   // Core Arduino functionalities
#include <IPAddress.h> // IP Address utility class

/* =======================
 * Macros
 * =======================
 * Define network and broker configurations.
 */
#define WIFI_SSID ""          /**< WiFi network SSID */
#define WIFI_PASSWORD ""      /**< WiFi network password */
#define MQTT_BROKER_PORT 1883 /**< MQTT broker listening port */

/* =======================
 * Enums
 * =======================
 * Enumerations for MQTT topics, providing a clear mapping
 * of sensor and actuator topics to indices.
 */

/**
 * Enum for sensor-related MQTT topics.
 */
enum class SensorTopic
{
    RACK0_WATER_TEMP, /**< Water temperature sensor topic */
    RACK0_AMB_TEMP,   /**< Ambient temperature sensor topic */
    RACK0_HUMIDITY,   /**< Ambient humidity sensor topic */
    RACK0_PH,         /**< Water pH sensor topic */
    RACK0_TDS,        /**< Total Dissolved Solids sensor topic */
    RACK0_EC,         /**< Electrical Conductivity sensor topic */
    SENSOR_COUNT      /**< Total number of sensor topics */
};

/**
 * Enum for actuator-related MQTT topics.
 */
enum class ActuatorTopic
{
    RACK0_WATERING0,          /**< Watering actuator topic */
    RACK0_DOSE_PUMP0,         /**< Dosing pump 0 actuator topic */
    RACK0_DOSE_PUMP1,         /**< Dosing pump 1 actuator topic */
    RACK0_DOSE_PUMP2,         /**< Dosing pump 2 actuator topic */
    RACK0_LIGHT_CONTROL0,     /**< Light control actuator topic */
    RACK0_FAN_CONTROL0,       /**< Fan control 0 actuator topic */
    RACK0_FAN_CONTROL1,       /**< Fan control 1 actuator topic */
    RACK0_HUMIDIFIER_CONTROL, /**< Humidifier control actuator topic */
    ACTUATOR_COUNT            /**< Total number of actuator topics */
};

/* =======================
 * Static IP Configuration
 * =======================
 * Declare external variables for network settings.
 * These will be defined in the corresponding .cpp file.
 */
extern IPAddress local_IP; /**< Static local IP address */
extern IPAddress gateway;  /**< Network gateway */
extern IPAddress subnet;   /**< Network subnet mask */
extern IPAddress dns;      /**< DNS server address */

/* =======================
 * Function Prototypes
 * =======================
 * Declare utility functions for topic handling.
 */

/**
 * Retrieves the MQTT topic string for a specific sensor.
 * @param sensor SensorTopic enum value.
 * @return Corresponding topic string.
 */
std::string get_sensor_topic(SensorTopic sensor);

/**
 * Retrieves the MQTT topic string for a specific actuator.
 * @param actuator ActuatorTopic enum value.
 * @return Corresponding topic string.
 */
std::string get_actuator_topic(ActuatorTopic actuator);

/**
 * Handles incoming messages for actuator topics.
 * Sends payload data to the appropriate actuator device via UART.
 * @param topic The MQTT topic string.
 * @param payload The message payload associated with the topic.
 */
void handleActuatorTopic(const char *topic, const char *payload);

/**
 * Handles incoming messages for sensor topics.
 * Sends payload data to the appropriate sensor device via UART.
 * @param topic The MQTT topic string.
 * @param payload The message payload associated with the topic.
 */
void handleSensorTopic(const char *topic, const char *payload);

/* =======================
 * MyMQTT Class
 * =======================
 * Custom MQTT server class that extends PicoMQTT::Server.
 * Includes additional functionality for topic subscriptions and UART handling.
 */
class MyMQTT : public PicoMQTT::Server
{
public:
    /**
     * Constructor for MyMQTT.
     * Initializes the MQTT server with a specified port.
     * @param port The MQTT broker port.
     */
    MyMQTT(uint16_t port) : PicoMQTT::Server(port) {}

    /**
     * Subscribes to all predefined MQTT topics.
     */
    void subscribeToTopics();

    /**
     * Adds a custom topic to the subscription list.
     * @param topic The MQTT topic to subscribe to.
     */
    void addTopicToSubscription(const char *topic);

    /**
     * Forwards UART data as MQTT messages.
     * Parses incoming data for topic and payload.
     * @param serialPort The UART port for communication.
     * @param broker The MQTT server instance.
     */
    void UART_MQTT(HardwareSerial &serialPort, MyMQTT &broker);

protected:
    /**
     * Overrides the authentication mechanism for the MQTT server.
     * Validates client credentials (username and password).
     * @param client_id The client identifier.
     * @param username The username for authentication.
     * @param password The password for authentication.
     * @return Connection return code indicating authentication status.
     */
    virtual PicoMQTT::ConnectReturnCode auth(const char *client_id, const char *username, const char *password) override;
};

#endif /* UTILS_H_ */
