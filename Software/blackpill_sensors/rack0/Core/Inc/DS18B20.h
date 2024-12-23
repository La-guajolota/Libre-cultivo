/*
 * DS18B20.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Adrián Silva Palafox
 *
 *  Description:
 *  This header file contains the definitions, constants, and function prototypes
 *  required for interfacing with the DS18B20 digital temperature sensor using
 *  a GPIO pin in a single-wire communication mode.
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

// Includes
#include "stm32f4xx_hal.h"  // HAL library for STM32
#include "utils.h"          // Utility functions for delay, etc.

/***************************
 * DEFINES
 ***************************/

/**
 * GPIO port and pin where the DS18B20 is connected.
 * Modify according to your hardware configuration.
 */
#define DS18B20_PORT GPIOA
#define DS18B20_PIN  GPIO_PIN_2

/****************************
 * FUNCTION PROTOTYPES
 ****************************/

/**
 * @brief Initializes communication with the DS18B20 sensor by sending a reset pulse
 *        and checking for a presence pulse.
 * @return
 *   - 1: Presence pulse detected, DS18B20 is ready.
 *   - -1: No presence pulse detected, DS18B20 not responding.
 */
uint8_t DS18B20_Start(void);

/**
 * @brief Sends an 8-bit command to the DS18B20 sensor over the one-wire interface.
 * @param data The 8-bit command or data to be written to the sensor.
 */
void DS18B20_Write(uint8_t data);

/**
 * @brief Reads an 8-bit value from the DS18B20 sensor over the one-wire interface.
 * @return The 8-bit value read from the sensor.
 */
uint8_t DS18B20_Read(void);

#endif /* INC_DS18B20_H_ */

