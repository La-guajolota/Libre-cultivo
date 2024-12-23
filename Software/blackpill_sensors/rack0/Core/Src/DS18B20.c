/*
 * DS18B20.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Adrián Silva Palafox
 *
 *  Description:
 *  This source file contains the implementation of functions for interfacing with
 *  the DS18B20 temperature sensor. The DS18B20 communicates using a one-wire protocol,
 *  which requires precise timing for proper operation.
 */

#include "DS18B20.h"

/*******************************
 * STATIC HELPER FUNCTIONS
 *******************************/

/**
 * @brief Configures the specified GPIO pin as an output.
 * @param GPIOx The GPIO port (e.g., GPIOA).
 * @param GPIO_Pin The GPIO pin number (e.g., GPIO_PIN_2).
 */
static void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
 * @brief Configures the specified GPIO pin as an input.
 * @param GPIOx The GPIO port (e.g., GPIOA).
 * @param GPIO_Pin The GPIO pin number (e.g., GPIO_PIN_2).
 */
static void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/*******************************
 * FUNCTION DEFINITIONS
 *******************************/

uint8_t DS18B20_Start(void) {
    uint8_t Response = 0;

    // Send reset pulse
    Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);   // Configure as output
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // Pull the line low
    delay_us(480);   // Hold the line low for 480 µs

    // Wait for the presence pulse
    Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);    // Configure as input
    delay_us(80);

    if (!(HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN))) {
        Response = 1;    // Presence pulse detected (line pulled low by DS18B20)
    } else {
        Response = -1;   // No presence pulse detected
    }

    delay_us(400);   // Allow the rest of the reset period to complete

    return Response;
}

void DS18B20_Write(uint8_t data) {
    // Write each bit of the byte
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i)) {  // If the bit is 1
            Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // Pull the line low
            delay_us(1);  // Short low pulse
            Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // Release the line
            delay_us(50);  // Hold high for the rest of the slot
        } else {  // If the bit is 0
            Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // Pull the line low
            delay_us(50);  // Hold low for the entire slot
            Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // Release the line
        }
    }
}

uint8_t DS18B20_Read(void) {
    uint8_t value = 0;

    // Read each bit of the byte
    for (int i = 0; i < 8; i++) {
        Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
        HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // Pull the line low
        delay_us(2);  // Short low pulse
        Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // Release the line
        if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)) {  // If the line is high
            value |= (1 << i);  // Set the corresponding bit
        }
        delay_us(60);  // Wait for the rest of the time slot
    }

    return value;
}

