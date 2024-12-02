/*
 * @version  : 0.1 RACK0
 * @author   : Adrián Silva Palafox
 * @company  : Fourier Embeds | Libre Cultivo
 * @brief    : Main application file for setting up an MQTT server with Wi-Fi connectivity.
 */

/* =======================
 * Libraries
 * =======================
 * Includes required libraries for Wi-Fi, MQTT, and Serial communication.
 */
#include <Arduino.h>
#include <WiFi.h>
#include "utils.h"

/* =======================
 * Macros
 * =======================
 * Pin configurations for Serial communication.
 */
#define SERIAL2_RX_PIN 16 /**< RX pin for Serial2 (Sensors) */
#define SERIAL2_TX_PIN 17 /**< TX pin for Serial2 (Sensors) */
#define SERIAL1_RX_PIN 4  /**< RX pin for Serial1 (Actuators) */
#define SERIAL1_TX_PIN 5  /**< TX pin for Serial1 (Actuators) */

/* =======================
 * Global Variables
 * =======================
 * Flags to indicate new data availability on respective Serial ports.
 */
volatile bool newDataSerial1 = false; /**< Flag for new data on Serial1 */
volatile bool newDataSerial2 = false; /**< Flag for new data on Serial2 */

/* =======================
 * Instances
 * =======================
 * MQTT server instance configured with the broker port.
 */
MyMQTT myMQTTServer(MQTT_BROKER_PORT); /**< MQTT Server instance */

/* =======================
 * Function Declarations
 * =======================
 * Prototypes for interrupt routines and utility functions.
 */
void serial1InterruptRoutine();
void serial2InterruptRoutine();
void checkPCSerial();

/* =======================
 * Setup Function
 * =======================
 * Initializes serial ports, Wi-Fi, and MQTT server.
 */
void setup()
{
    // Initialize serial ports
    Serial.begin(115200);                                              // PC Serial
    Serial1.begin(115200, SERIAL_8N1, SERIAL1_RX_PIN, SERIAL1_TX_PIN); // Serial1 for Actuators
    Serial2.begin(115200, SERIAL_8N1, SERIAL2_RX_PIN, SERIAL2_TX_PIN); // Serial2 for Sensors

    // Attach interrupts for Serial RX
    attachInterrupt(digitalPinToInterrupt(SERIAL1_RX_PIN), serial1InterruptRoutine, FALLING);
    attachInterrupt(digitalPinToInterrupt(SERIAL2_RX_PIN), serial2InterruptRoutine, FALLING);

    // Configure static IP for Wi-Fi
    if (!WiFi.config(local_IP, gateway, subnet, dns))
    {
        Serial.println("Static IP configuration failed");
    }

    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA); // Set Wi-Fi mode to station
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.printf("Wi-Fi connected! IP: %s\n", WiFi.localIP().toString().c_str());

    // Initialize MQTT server
    myMQTTServer.subscribeToTopics(); // Subscribe to predefined topics
    myMQTTServer.begin();             // Start the MQTT server
}

/* =======================
 * Loop Function
 * =======================
 * Continuously checks for new data on Serial ports and maintains MQTT server.
 */
void loop()
{
    // Maintain MQTT server
    myMQTTServer.loop();

    // Handle new data from Serial1 (Actuators)
    if (newDataSerial1)
    {
        myMQTTServer.UART_MQTT(Serial1, myMQTTServer); // Process Serial1 data
        newDataSerial1 = false;                        // Reset flag
    }

    // Handle new data from Serial2 (Sensors)
    if (newDataSerial2)
    {
        myMQTTServer.UART_MQTT(Serial2, myMQTTServer); // Process Serial2 data
        newDataSerial2 = false;                        // Reset flag
    }

    // Handle PC Serial data (debugging or additional commands)
    checkPCSerial();
}

/* =======================
 * Interrupt Service Routines
 * =======================
 * Triggered on receiving data on respective Serial ports.
 */

/**
 * ISR for Serial1 (Actuators).
 * Sets the flag indicating new data is available on Serial1.
 */
void serial1InterruptRoutine()
{
    newDataSerial1 = true; // Set flag for Serial1
}

/**
 * ISR for Serial2 (Sensors).
 * Sets the flag indicating new data is available on Serial2.
 */
void serial2InterruptRoutine()
{
    newDataSerial2 = true; // Set flag for Serial2
}

/* =======================
 * Utility Functions
 * =======================
 * Additional functionality for handling PC Serial input.
 */

/**
 * Reads and processes data from the PC Serial port.
 * Sends the data as MQTT messages to the broker.
 */
void checkPCSerial()
{
    if (Serial.available() > 0) // Check if data is available
    {
        myMQTTServer.UART_MQTT(Serial, myMQTTServer); // Process PC Serial data
    }
}
