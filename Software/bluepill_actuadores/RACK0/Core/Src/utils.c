/*
 * utils.c
 *
 * Created on: Oct 4, 2024
 * Author: Adrián Silva Palafox
 * Company: Fourier Embeds | Libre Cultivo
 * Description: Utility functions for handling sensor and actuator topics,
 *              publishing MQTT messages, and providing a microsecond delay function.
 *              These functions are designed for embedded systems, specifically for
 *              communication over UART and sensor handling in an embedded environment.
 */
#ifndef SRC_UTILS_C_
#define SRC_UTILS_C_

#include "utils.h"

// ---------------------------
// List of Valid Topics
// ---------------------------

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

typedef enum {
    WATER_TEMPERATURE = 0,
    AMBIENT_TEMPERATURE,
    AMBIENT_HUMIDITY,
    WATER_PH,
    WATER_TDS,
    WATER_EC,
    NUM_SENSOR_TOPICS
} TopicSensorIndex;

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

typedef enum {
    WATERING = 0,
    DOSE_PUMP0,
    DOSE_PUMP1,
    DOSE_PUMP2,
    LIGHT_CONTROL,
    FAN_CONTROL0,
    FAN_CONTROL1,
    HUMIDIFIER,
    NUM_TOPICS
} TopicActuatorIndex;

#else
#error "Either DAQ or ACT must be defined."
#endif

// ---------------------------
// Function Definitions
// ---------------------------

/**
 * @brief Delays execution for a specified number of microseconds.
 *
 * This function uses a hardware timer (TIM2) to provide a precise delay
 * in microseconds. The counter of the timer is set to 0, and the function
 * waits until the timer reaches the desired delay.
 *
 * @param us The number of microseconds to delay.
 */
void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);  // Set the counter value to 0
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);  // Wait for the counter to reach the specified delay
}

/**
 * @brief Publishes a message with a given topic and floating-point value over UART.
 *
 * This function formats a message with the given topic and value, then
 * transmits it over UART (USART1). The format is: <topic>*<value>\r\n.
 *
 * @param topic The MQTT topic to send.
 * @param val The floating-point value to include in the message.
 */
void publishTopic(const char* topic, float val)
{
    char uart_buf[50] = {0};
    sprintf(uart_buf, "%s*%.2f\r\n", topic, val);  // Format the topic and value into the buffer

    size_t len = strlen(uart_buf);  // Calculate the length of the message

    HAL_UART_Transmit(&huart1, (uint8_t*)uart_buf, len, HAL_MAX_DELAY);  // Transmit the message over UART

    HAL_Delay(3000);  // Optional delay after publishing the message (adjustable based on needs)
}

/**
 * @brief Identifies if the topic provided exists in the list of valid topics.
 *
 * This function checks if the given topic is part of the predefined valid topics.
 *
 * @param topic The topic to check.
 * @return ERROR_CODE Returns SUCCESS if valid, otherwise returns UNKNOWN_TOPIC.
 */
static ERROR_CODE identifyTopic(const char *topic)
{
    for (int i = 0; i < num_valid_topics; i++) {
        if (strcmp(topic, valid_topics[i]) == 0) {
            return SUCCESS;  // Topic is valid
        }
    }
    return UNKNOWN_TOPIC;  // Topic not found
}

/**
 * @brief Receives a message with topic and value, and parses the value from the string.
 *
 * This function searches for the '*' character to separate the topic and value,
 * then converts the value from string to float. It also verifies that the topic is valid.
 *
 * @param msg The message string.
 * @param val Pointer to the float where the value will be stored.
 * @return ERROR_CODE Returns SUCCESS if the message is valid and successfully parsed,
 *         or an error code indicating failure.
 */
ERROR_CODE reciveTopic(char *msg, float *val)
{
    // Search for the '*' character
    char *pos = strchr(msg, '*');

    if (pos != NULL) {
        // Calculate the index (position) of the '*' character
        int8_t index = pos - msg;

        // Separate the topic and data
        char topic[50] = {0};  // Buffer for the topic
        char data[4] = {0};    // Buffer for the data (3 characters + '\0')

        // Copy the topic up to the '*' character
        strncpy(topic, msg, index);
        topic[index] = '\0';  // Ensure null-termination

        // Copy the data after '*' (3 characters maximum)
        strncpy(data, pos + 1, 3);
        data[3] = '\0';  // Ensure null-termination

        // Convert data to a numeric value
        *val = atof(data);

        // Identify the topic
        return identifyTopic(topic);
    }

    // If '*' character is not found, return error
    return CHAR_NOT_FOUND;
}

/**
 * @brief Handles actuator control commands based on actuator ID and value.
 *
 * This function performs control actions for various actuators (e.g., pumps, lights, fans).
 *
 * @param actu The actuator ID.
 * @param val The value to set (e.g., PWM duty cycle or actuator state).
 * @return ERROR_CODE Returns SUCCESS if the actuator was handled,
 *         or an error code if the actuator ID is unknown.
 */
#ifdef DAQ
// Using a STM32F4 as a DAQ
#elif defined(ACT)
// Using a STM32F1 as an ACTU

ERROR_CODE actuatorMotorsHandler(uint8_t actu, uint8_t val)
{
	switch (actu) {
	    case WATERING:
	        if (val) {  // PUMP IS ON
	            HAL_GPIO_WritePin(WATgpio_GPIO_Port, WATgpio_Pin, 0);
	        } else {  // PUMP IS OFF
	            HAL_GPIO_WritePin(WATgpio_GPIO_Port, WATgpio_Pin, 1);
	        }
	        break;

	    case DOSE_PUMP0:
	        TIM3->CCR3 = val;  // Set PWM for dose pump 0
	        break;

	    case DOSE_PUMP1:
	        TIM3->CCR4 = val;  // Set PWM for dose pump 1
	        break;

	    case DOSE_PUMP2:
	        TIM4->CCR1 = val;  // Set PWM for dose pump 2
	        break;

	    case LIGHT_CONTROL:
	        TIM4->CCR2 = val;  // Set PWM for light control
	        break;

	    case FAN_CONTROL0:
	        TIM4->CCR3 = val;  // Set PWM for fan 0
	        break;

	    case FAN_CONTROL1:
	        TIM4->CCR4 = val;  // Set PWM for fan 1
	        break;

	    case HUMIDIFIER: 	   // Send pulse to Humidifier
	        HAL_GPIO_TogglePin(HUMgpio_GPIO_Port, HUMgpio_Pin);
	    default:
	        return UNKNOWN_ACTUATOR;  // Invalid actuator
	}
    return SUCCESS;
}

#else
#error "Either DAQ or ACT must be defined."
#endif

#endif /* SRC_UTILS_C_ */
