/*
 * DHT11_22.c
 *
 *  Created on: Apr 19, 2024
 *      Author: Adrian Silva Palafox
 *
 *  Description: This file implements the functions for controlling the DHT11
 *  and DHT22 sensors. The functions handle initialization, response verification,
 *  and data reading. The code is optimized for embedded systems, using delays
 *  with timers to avoid blocking the microcontroller.
 */

#include "DHT11_22.h"

/* Definitions for DHT data pin */
#define DATA_PORT DHTdata_GPIO_Port
#define DATA_PIN DHTdata_Pin

/**
 * @brief Set the data pin as output.
 *
 * This function configures the data pin as an output to send signals to the sensor.
 *
 * @param GPIO The GPIO port.
 * @param GPIO_pin The GPIO pin to configure.
 */
void Set_Pin_Output(GPIO_TypeDef *GPIO, uint16_t GPIO_pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO, &GPIO_InitStruct);
}

/**
 * @brief Set the data pin as input.
 *
 * This function configures the data pin as an input to read data from the sensor.
 *
 * @param GPIO The GPIO port.
 * @param GPIO_pin The GPIO pin to configure.
 */
void Set_Pin_Input(GPIO_TypeDef *GPIO, uint16_t GPIO_pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIO, &GPIO_InitStruct);
}

/**
 * @brief Initializes the DHT sensor (DHT11 or DHT22).
 *
 * This function configures the data pin for communication with the sensor.
 * It first pulls the pin low for a specific period, then switches it to input
 * mode to receive the data.
 */
void DHT_init(void)
{
    Set_Pin_Output(DATA_PORT, DATA_PIN);  // Set the pin as output
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, 0);   // Pull the pin low

    #if DHT_SENSOR_TYPE == DHT11
    	delay_us(18000);  // Wait for 18ms (DHT11)
    #else
    	delay_us(1200);   // Wait for > 1ms (DHT22)
    #endif

    Set_Pin_Input(DATA_PORT, DATA_PIN);   // Set the pin as input
}

/**
 * @brief Checks the response from the sensor.
 *
 * This function waits for the sensor to pull the data line low, then high,
 * indicating it is ready to send data. If the response is valid, it returns 1;
 * otherwise, it returns -1.
 *
 * @return 1 if the response is valid, -1 if invalid.
 */
uint8_t DHT_Check_Response(void)
{
    Set_Pin_Input(DATA_PORT, DATA_PIN);   // Set as input
    uint8_t Response = 0;
    delay_us(40);  // Wait for 40us
    if (!(HAL_GPIO_ReadPin(DATA_PORT, DATA_PIN)))  // If the pin is low
    {
    	delay_us(80);   // Wait for 80us
        if ((HAL_GPIO_ReadPin(DATA_PORT, DATA_PIN))) Response = 1;  // Valid response
        else Response = -1;  // Invalid response
    }

    // Wait until the pin goes low again
    while ((HAL_GPIO_ReadPin(DATA_PORT, DATA_PIN)));

    return Response;
}

/**
 * @brief Reads one byte of data from the sensor.
 *
 * This function reads 8 bits from the sensor, waits for the data pin to go high
 * and then low again for each bit, and returns the byte.
 *
 * @return The byte read from the sensor.
 */
uint8_t DHT_Read(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        // Wait for the pin to go high (start of bit)
        while (!(HAL_GPIO_ReadPin(DATA_PORT, DATA_PIN)));

        delay_us(40);  // Wait for 40 us (bit time)
        if (!(HAL_GPIO_ReadPin(DATA_PORT, DATA_PIN)))   // If the pin is low
        {
            i &= ~(1 << (7 - j));   // Write 0 to the bit
        }
        else
        {
            i |= (1 << (7 - j));   // Write 1 to the bit
        }

        // Wait for the pin to go low after the bit is read
        while ((HAL_GPIO_ReadPin(DATA_PORT, DATA_PIN)));
    }

    return i;
}

