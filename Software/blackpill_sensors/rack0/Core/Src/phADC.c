/*
 * phADC.c
 *
 *  Created on: Nov 19, 2024
 *      Author: Adrián Silva Palafox
 *
 *  This file contains the implementation of functions for initializing and reading
 *  the pH sensor. The pH sensor uses an ADC for voltage readings, and the pH value
 *  is calculated using linear regression based on known buffer points.
 */

#include "phADC.h"
#include "utils.h"

// Static function to sort ADC readings using Bubble Sort for removing outliers
static void bubbleSort(float *arr, int size) {
    bool swapped;  // Flag to track if any swap occurred during the pass
    float *ptr1, *ptr2;  // Pointers to compare and swap adjacent elements

    // Traverse the array and compare adjacent elements
    for (int i = 0; i < size - 1; i++) {
        swapped = false;  // Reset the swap flag for each pass
        ptr1 = arr;  // Start comparison from the first element

        // Loop through the unsorted portion of the array
        for (int j = 0; j < size - i - 1; j++) {
            ptr2 = ptr1 + 1;  // Pointer to the next element

            // Swap if the current element is greater than the next
            if (*ptr1 > *ptr2) {
                float temp = *ptr1;
                *ptr1 = *ptr2;
                *ptr2 = temp;
                swapped = true;  // Mark that a swap occurred
            }
            ptr1++;  // Move to the next element
        }

        // If no swap occurred, the array is already sorted, break early
        if (!swapped) break;
    }
}

// Function to calculate the linear regression (slope and intercept) using the least squares method
static void linearRegression(float x1, float y1, float x2, float y2, float x3, float y3, float *m, float *b) {
    // Calculate the necessary sums for linear regression (slope and intercept)
    float sum_x = x1 + x2 + x3;  // Sum of x-values
    float sum_y = y1 + y2 + y3;  // Sum of y-values
    float sum_x2 = x1 * x1 + x2 * x2 + x3 * x3;  // Sum of x^2 values
    float sum_xy = x1 * y1 + x2 * y2 + x3 * y3;  // Sum of x*y values

    int n = 3;  // Number of data points used for regression (fixed to 3)

    // Calculate the slope (m) using the least squares formula
    *m = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    // Calculate the intercept (b) using the least squares formula
    *b = (sum_y - *m * sum_x) / n;
}

// Function to initialize the pH sensor by calculating the slope and intercept
// based on the known buffer points (A, B, C) and their corresponding voltages.
void initPHsensor(PHsensor *sensor, float Avolts, float Bvolts, float Cvolts) {
    float mBuffer, bBuffer;

    // Perform linear regression to find the slope (m) and intercept (b)
    linearRegression(Avolts, BUFFER_A, Bvolts, BUFFER_B, Cvolts, BUFFER_C , &mBuffer, &bBuffer);

    // Store the calculated slope and intercept in the sensor structure
    sensor->m = mBuffer;
    sensor->b = bBuffer;
}

// Function to read multiple ADC values and return the filtered pH value
static void readADC(float *adc, uint8_t samples) {
    // Collect 'samples' number of ADC readings
    for (int i = 0; i < samples; ++i) {
        HAL_ADC_Start(&hadc1);  // Start the ADC conversion
        if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {  // Wait for conversion to complete
            uint32_t rawADC = HAL_ADC_GetValue(&hadc1);  // Get the raw ADC value
            // Convert ADC value to voltage (scaled based on ADC resolution)
            adc[i] = rawADC * 3.3/4096.0;
        }
        HAL_Delay(50);  // Short delay between samples
    }
}

// Function to read and calculate the pH value based on filtered ADC readings
void readPH(PHsensor *sensor) {
    float adc[SAMPLINGS] = {0};  // Array to hold the ADC readings

    // Read ADC values from the sensor
    readADC(adc, SAMPLINGS);

    // Sort the ADC readings to remove outliers
    bubbleSort(adc, SAMPLINGS);

    // Calculate the average of the 2 middle values after sorting
    float adcFiltered = (adc[MIDDLE_B] + adc[MIDDLE_A]) / 2.0;

    // Calculate the pH value using the linear model: pH = m * ADC + b
    sensor->ph = adcFiltered * sensor->m + sensor->b;
}

