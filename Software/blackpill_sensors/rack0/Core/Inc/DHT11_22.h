/*
 * DHT11_22.h
 *
 *  Created on: Apr 19, 2024
 *      Author: Adrian Silva Palafox
 *
 *  Description: This file contains the necessary declarations for initializing
 *  and reading from the DHT11 and DHT22 sensors. Both temperature and humidity
 *  sensors are handled similarly, with general functions for configuration and
 *  data reading.
 */

#ifndef INC_DHT11_22_H_
#define INC_DHT11_22_H_

#include "main.h"
#include "utils.h"

/* Define sensor type: DHT11 or DHT22 */
#define DHT_SENSOR_TYPE  DHT22  // Change to DHT22 if using the DHT22 sensor

/*
 * Functions
 */

/**
 * @brief Initializes the DHT sensor (DHT11 or DHT22).
 *
 * Configures the data pin as output to send the start pulse, then switches it to
 * input to receive the data from the sensor.
 */
void DHT_init(void);

/**
 * @brief Checks the response from the sensor.
 *
 * @return 1 if the response is valid, -1 if the response is invalid.
 */
uint8_t DHT_Check_Response(void);

/**
 * @brief Reads one byte of data from the sensor.
 *
 * Reads 8 bits from the sensor and returns them as a byte.
 *
 * @return The byte read from the sensor.
 */
uint8_t DHT_Read(void);

#endif /* INC_DHT11_22_H_ */
