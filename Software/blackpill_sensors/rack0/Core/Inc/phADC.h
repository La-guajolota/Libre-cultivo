/*
 * phADC.h
 *
 *  Created on: Nov 19, 2024
 *      Author: Adrián Silva Palafox
 *
 *  This header file contains the definitions, constants, and function prototypes
 *  for the pH sensor interface using an ADC and linear regression to calculate
 *  the pH value based on ADC voltage readings.
 */

#ifndef INC_PHADC_H_
#define INC_PHADC_H_

// Includes
#include "utils.h"

// Macros
#define BUFFER_A 4.01    // pH buffer point A voltage (for calibration)
#define BUFFER_B 6.86    // pH buffer point B voltage (for calibration)
#define BUFFER_C 9.18    // pH buffer point C voltage (for calibration)

#define SAMPLINGS 20     // Number of ADC samples to average for more accurate readings
#define MIDDLE_A (SAMPLINGS/2) //Used in media filter
#define MIDDLE_B (MIDDLE_A-1)  //Used in media filter

// Data types
typedef struct {
    float ph;  // The calculated pH value based on ADC readings
    float m;   // The slope (m) of the linear regression equation
    float b;   // The intercept (b) of the linear regression equation
} PHsensor;

// Function prototypes
/**
 * @brief Initializes the pH sensor by calculating the slope (m) and intercept (b)
 *        using a linear regression based on known buffer voltages (A, B, C) and
 *        their corresponding pH values.
 *
 * @param sensor Pointer to the PHsensor structure that will store the slope and intercept.
 * @param Avolts Voltage corresponding to pH 4.01 (Buffer A).
 * @param Bvolts Voltage corresponding to pH 6.86 (Buffer B).
 * @param Cvolts Voltage corresponding to pH 9.18 (Buffer C).
 */
void initPHsensor(PHsensor *sensor, float Avolts, float Bvolts, float Cvolts);

/**
 * @brief Reads the pH value by sampling ADC readings, filtering them, and calculating
 *        the pH based on the linear regression parameters stored in the sensor structure.
 *
 * @param sensor Pointer to the PHsensor structure which contains the linear regression
 *               parameters (slope and intercept).
 */
void readPH(PHsensor *sensor);

#endif /* INC_PHADC_H_ */
