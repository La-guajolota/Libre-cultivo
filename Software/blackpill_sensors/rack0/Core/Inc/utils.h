/*
* utils.h
*
*  Created on: Oct 4, 2024
*  	   Company: Fourier Embeds | Libre Cultivo
*      Author: Adrián Silva Palafox
*      Description: Header file for utility functions and constants used in the embedded system.
*                   This file contains declarations for sensor and actuator topics, a
*                   microsecond delay function, and a function for publishing messages over UART.
*                   It also includes the necessary external definitions for hardware peripherals.
*/

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

// NATIVE STM32 FRAMEWORK INCLUDES
#include "stm32f4xx_hal.h" // HAL library for STM32F4
//#include "stm32f1xx_hal.h" // HAL library for STM32F1

// STANDARD C LIBRARY
#include <stdint.h>          // Standard integer types
#include <stdio.h>           // Standard input/output for sprintf
#include <string.h>          // Standard string handling functions (strlen, etc.)
#include <stdbool.h>
#include <stdlib.h>

// --------------------
// OPERATION MODE MACROS
// --------------------
// Uncomment the macro corresponding to the intended system mode:
// --------------------
#define DAQ       // Library running on a Data Acquisition device
//#define ACT      // Library running on an Actuator Control system

typedef int8_t ERROR_CODE;
// --------------------
// ERROR CONTROL MACROS
// --------------------
#define SUCCESS_           0   // Operation successful
#define ERROR_            -1   // Generic error code
#define CHAR_NOT_FOUND_   -2   // Specified character not found in string
#define UNKNOWN_TOPIC     -3   // Unrecognized topic for this system


// MQTT Topics Definitions
/* Sensor Topics:
* These topics represent the MQTT communication topics for sensor data collection.
* The topics correspond to various sensors such as temperature, humidity, pH, TDS, and EC.
*/
#define TOPIC_WATER_TEMPERATURE "rack0/sens/water/temperature"
#define TOPIC_AMBIENT_TEMPERATURE "rack0/sens/ambient/temperature"
#define TOPIC_AMBIENT_HUMIDITY "rack0/sens/ambient/humidity"
#define TOPIC_WATER_PH "rack0/sens/water/ph"
#define TOPIC_WATER_TDS "rack0/sens/water/tds"
#define TOPIC_WATER_EC "rack0/sens/water/ec"

/* Actuator Topics:
* These topics represent the MQTT communication topics for controlling actuators.
* The actuators include systems like watering, dosing pumps, lights, fans, and a humidifier.
*/
#define TOPIC_WATERING "rack0/actu/watering0"
#define TOPIC_DOSE_PUMP0 "rack0/actu/dose_pump0"
#define TOPIC_DOSE_PUMP1 "rack0/actu/dose_pump1"
#define TOPIC_DOSE_PUMP2 "rack0/actu/dose_pump2"
#define TOPIC_LIGHT_CONTROL "rack0/actu/light/control"
#define TOPIC_FAN_CONTROL0 "rack0/actu/fan/control0"
#define TOPIC_FAN_CONTROL1 "rack0/actu/fan/control1"
#define TOPIC_HUMIDIFIER "rack0/actu/humidifier"

// List of HW peripherals used
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim11;
extern UART_HandleTypeDef huart1;

// ------------------------
// FUNCTION PROTOTYPES
// ------------------------
void delay_us(uint16_t us);    // Function to delay execution for specified microseconds

void publishTopic(const char* topic, float val); // Function to publish a message with topic and value
ERROR_CODE reciveTopic(char *msg, float *val);  // Function to receive a message with topic and value

#endif /* INC_UTILS_H_ */
