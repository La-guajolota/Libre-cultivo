/*
 * utils.c
 *
 *  Created on: Oct 4, 2024
 *      Author: Adrián Silva Palafox
 *      Company: Fourier Embeds | Libre Cultivo
 *      Description: Utility functions for handling sensor and actuator topics,
 *                   publishing MQTT messages, and providing a microsecond delay function.
 *                   These functions are designed for embedded systems, specifically for
 *                   communication over UART and sensor handling in an embedded environment.
 */

#ifndef SRC_UTILS_C_
#define SRC_UTILS_C_

#include "utils.h"

/*
 * List of Valid Topics
 * This array includes all predefined topics for either sensors (DAQ) or actuators (ACT),
 * depending on the compilation flags.
 */
#ifdef DAQ
/* Sensor Topics Array */
const char* valid_topics[] = {
    TOPIC_WATER_TEMPERATURE,
    TOPIC_AMBIENT_TEMPERATURE,
    TOPIC_AMBIENT_HUMIDITY,
    TOPIC_WATER_PH,
    TOPIC_WATER_TDS,
    TOPIC_WATER_EC
};
size_t num_valid_topics = sizeof(valid_topics) / sizeof(valid_topics[0]);

#elif defined(ACT)
/* Actuator Topics Array */
const char* valid_topics[] = {
    TOPIC_WATERING,
    TOPIC_DOSE_PUMP0,
    TOPIC_DOSE_PUMP1,
    TOPIC_DOSE_PUMP2,
    TOPIC_LIGHT_CONTROL,
    TOPIC_FAN_CONTROL0,
    TOPIC_FAN_CONTROL1,
    TOPIC_HUMIDIFIER
};
size_t num_valid_topics = sizeof(valid_topics) / sizeof(valid_topics[0]);

#else
#error "Either DAQ or ACT must be defined."
#endif

/* Function Definitions */

/**
 * @brief Delays execution for a specified number of microseconds.
 *
 * This function uses a hardware timer (TIM11) to provide a precise delay
 * in microseconds. The counter of the timer is set to 0, and the function
 * waits until the timer reaches the desired delay.
 *
 * @param us The number of microseconds to delay.
 * @note This function assumes that TIM11 has been properly initialized in the system.
 */
void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim11, 0);  // Set the counter value to 0
    while (__HAL_TIM_GET_COUNTER(&htim11) < us);  // Wait for the counter to reach the specified delay
}

/**
 * @brief Publishes a message with a given topic and floating-point value over UART.
 *
 * This function formats a message with the given topic and value, then
 * transmits it over UART (USART1). The format is: <topic>*<value>\r\n.
 *
 * @param topic The MQTT topic to send.
 * @param val The floating-point value to include in the message.
 * @note The message will be transmitted in the format: "<topic>*<value>\r\n".
 * @note This function includes a 3-second delay after transmitting the message.
 */
void publishTopic(const char* topic, float val)
{
    char uart_buf[50] = {0};
    sprintf(uart_buf, "%s*%.2f\r\n", topic, val);  // Format the topic and value into the buffer

    size_t len = strlen(uart_buf);  // Calculate the length of the message

    HAL_UART_Transmit(&huart1, (uint8_t*)uart_buf, len, HAL_MAX_DELAY);  // Transmit the message over UART

    HAL_Delay(3000);  // Add a 3-second delay (optional, can be adjusted based on system needs)
}

// Function to identify a valid topic
static ERROR_CODE identifyTopic(const char *topic) {
    for (int i = 0; i < num_valid_topics; i++) {
        if (strcmp(topic, valid_topics[i]) == 0) {
            return SUCCESS; // Topic is valid
        }
    }
    return UNKNOWN_TOPIC; // Topic not found
}

/*
 * Function to receive and parse topic and value from a message.
 */
ERROR_CODE reciveTopic(char *msg, float *val) {
    // Search for the '*' character
    char *pos = strchr(msg, '*');

    if (pos != NULL) {
        // Calculate the index (position) of the '*' character
        int8_t index = pos - msg;

        // Separate the topic and data
        char topic[50] = {0};  // Buffer for the topic
        char data[4] = {0};    // Buffer for the data (3 characters + '\0')

        // Copy the topic
        strncpy(topic, msg, index);  // Copy up to the '*' character
        topic[index] = '\0';         // Ensure null-terminated string

        // Copy the data after '*'
        strncpy(data, pos + 1, 3);   // Copy 3 characters after '*'
        data[3] = '\0';             // Ensure null-terminated string

        // Convert data to a numeric value
        *val = atof(data);

        // Identify the topic
        return identifyTopic(topic);
    }

    // If '*' character is not found
    return CHAR_NOT_FOUND_;
}

ERROR_CODE actuatorMotorsHandler(uint8_t actu, uint8_t val){

}

#endif /* SRC_UTILS_C_ */
