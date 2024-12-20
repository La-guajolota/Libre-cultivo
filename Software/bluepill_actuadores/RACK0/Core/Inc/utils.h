/*
* utils.h
*
* Created on: Oct 4, 2024
* Company: Fourier Embeds | Libre Cultivo
* Author: Adrián Silva Palafox
* Description: Header file for utility functions and constants used in the embedded system.
*              It contains sensor and actuator topics, delay functions, and UART message handling.
*              External hardware peripheral definitions are included based on the system mode (DAQ or ACT).
*/

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

// STM32 HAL Library Include (STM32F1 or STM32F4)
#include "stm32f1xx_hal.h"  // Use this for STM32F1
// #include "stm32f4xx_hal.h"  // Uncomment for STM32F4
#include "main.h"

// Standard C Libraries
#include <stdint.h>          // For standard integer types
#include <stdio.h>           // For standard input/output (sprintf, etc.)
#include <string.h>          // For string manipulation (strchr, etc.)
#include <stdbool.h>         // For boolean type support
#include <stdlib.h>          // For standard functions (atoi, etc.)

// --------------------
// OPERATION MODE MACROS
// --------------------
// Define one of the following modes depending on the system:
// Uncomment for Data Acquisition (DAQ) mode
// #define DAQ

// Uncomment for Actuator Control (ACT) mode
#define ACT

// ----------------------
// ERROR CODE DEFINITIONS
// ----------------------
typedef int8_t ERROR_CODE;

// Error codes for the system
#define SUCCESS            0   // Operation successful
#define ERROR              -1  // General error
#define CHAR_NOT_FOUND     -2  // Character not found in string
#define UNKNOWN_TOPIC      -3  // Unrecognized topic
#define UNKNOWN_ACTUATOR   -4  // Unrecognized actuator
#define UNKNOWN_SENSOR     -5  // Unrecognized sensor

// ------------------------
// MQTT TOPIC DEFINITIONS
// ------------------------
// Sensor topics for MQTT communication
#define TOPIC_WATER_TEMPERATURE "rack0/sens/water/temperature"
#define TOPIC_AMBIENT_TEMPERATURE "rack0/sens/ambient/temperature"
#define TOPIC_AMBIENT_HUMIDITY "rack0/sens/ambient/humidity"
#define TOPIC_WATER_PH "rack0/sens/water/ph"
#define TOPIC_WATER_TDS "rack0/sens/water/tds"
#define TOPIC_WATER_EC "rack0/sens/water/ec"

// Actuator topics for MQTT communication
#define TOPIC_WATERING "rack0/actu/watering0"
#define TOPIC_DOSE_PUMP0 "rack0/actu/dose_pump0"
#define TOPIC_DOSE_PUMP1 "rack0/actu/dose_pump1"
#define TOPIC_DOSE_PUMP2 "rack0/actu/dose_pump2"
#define TOPIC_LIGHT_CONTROL "rack0/actu/light/control"
#define TOPIC_FAN_CONTROL0 "rack0/actu/fan/control0"
#define TOPIC_FAN_CONTROL1 "rack0/actu/fan/control1"
#define TOPIC_HUMIDIFIER "rack0/actu/humidifier"

// -----------------------
// PERIPHERAL DEFINITIONS
// -----------------------
#ifdef DAQ
// Using STM32F4 for Data Acquisition (DAQ)
extern ADC_HandleTypeDef hadc1;  // ADC for sensor readings
extern TIM_HandleTypeDef htim11; // Timer for time-based operations
extern UART_HandleTypeDef huart1; // UART for communication

#elif defined(ACT)
// Using STM32F1 for Actuator Control (ACT)
extern TIM_HandleTypeDef htim2;  // Timer for PWM or time-based control
extern UART_HandleTypeDef huart1; // UART for communication

extern TIM_HandleTypeDef htim3;  // Additional timers for control
extern TIM_HandleTypeDef htim4;  // Additional timers for control

#else
#error "Either DAQ or ACT must be defined."
#endif

// ------------------------
// FUNCTION PROTOTYPES
// ------------------------
// Microsecond delay function
void delay_us(uint16_t us);

// Function to publish MQTT messages with a topic and value
void publishTopic(const char* topic, float val);

// Function to receive MQTT messages with topic and value
ERROR_CODE receiveTopic(char *msg, float *val);

#ifdef DAQ
// DAQ-specific functions (if any)
#elif defined(ACT)
// ACT-specific functions (if any)
ERROR_CODE actuatorMotorsHandler(uint8_t actu, uint8_t val);
#else
#error "Either DAQ or ACT must be defined."
#endif

#endif /* INC_UTILS_H_ */
