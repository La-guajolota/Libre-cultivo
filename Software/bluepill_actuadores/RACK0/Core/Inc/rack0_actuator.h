/*
 * rack0_actuator.h
 *
 *  Created on: Nov 16, 2024
 *      Author: Adrián Silva Palafox
 *      Company: Fourier Embeds | Libre Cultivo
 *
 *  Description:
 *      Optimized header file for managing actuators (dose pumps, fans, supply pumps, and grow LEDs)
 *      in the Rack 0 actuator system. Provides data structures, macros, and function prototypes
 *      with enhanced memory efficiency and logical organization.
 */

#ifndef INC_RACK0_ACTUATOR_H_
#define INC_RACK0_ACTUATOR_H_

/******************************************************************************
 * SECTION 1: LIBRARIES
 *****************************************************************************/
// Standard C Libraries
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// STM32 HAL Libraries
#include "stm32f1xx_hal.h"
#include "main.h"

/******************************************************************************
 * SECTION 2: MACROS
 *****************************************************************************/

/** General Configuration */
#define MAX_ACTUATORS        8    // Maximum number of actuators supported
#define DEFAULT_PWM_DUTY     50   // Default PWM duty cycle (percentage)
#define PUMP_DEFAULT_TIME_ON 10   // Default ON time for pumps (in seconds)

/** State Macros */
#define STATE_ON             true
#define STATE_OFF            false

/******************************************************************************
 * SECTION 3: ENUMERATIONS
 *****************************************************************************/

/** Modes for multi-level actuators */
typedef enum {
    MODE_LOW = 0,
    MODE_MID,
    MODE_HIGH
} ActuatorMode;

/******************************************************************************
 * SECTION 4: DATA TYPES & STRUCTURES
 *****************************************************************************/

/** Generic actuator structure */
typedef struct {
    uint8_t ID;          // Unique identifier
    uint8_t pwmDuty;     // PWM duty cycle (0-100%)
    bool state;          // ON/OFF state
    ActuatorMode mode;   // Operational mode (specific actuators only)
} Actuator;

/** Rack structure to manage actuators */
typedef struct {
    Actuator actuators[MAX_ACTUATORS]; // Array of generic actuators
    uint8_t activeCount;               // Number of active actuators
} Rack0;

/******************************************************************************
 * SECTION 5: FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * @brief  Initialize the Rack 0 Actuator system.
 * @param  rack Pointer to the Rack0 instance.
 * @retval None
 */
void Rack0_Init(Rack0 *rack);

/**
 * @brief  Set the state of a specific actuator.
 * @param  rack Pointer to the Rack0 instance.
 * @param  actuatorID ID of the actuator to control.
 * @param  state Desired state (STATE_ON/STATE_OFF).
 * @retval true if successful, false otherwise.
 */
bool Rack0_SetActuatorState(Rack0 *rack, uint8_t actuatorID, bool state);

/**
 * @brief  Set the PWM duty cycle of a specific actuator.
 * @param  rack Pointer to the Rack0 instance.
 * @param  actuatorID ID of the actuator.
 * @param  pwmDuty Desired PWM duty cycle (0-100%).
 * @retval true if successful, false otherwise.
 */
bool Rack0_SetPWMDuty(Rack0 *rack, uint8_t actuatorID, uint8_t pwmDuty);

/**
 * @brief  Set the mode of a specific actuator (if applicable).
 * @param  rack Pointer to the Rack0 instance.
 * @param  actuatorID ID of the actuator.
 * @param  mode Desired mode (MODE_LOW, MODE_MID, MODE_HIGH).
 * @retval true if successful, false otherwise.
 */
bool Rack0_SetMode(Rack0 *rack, uint8_t actuatorID, ActuatorMode mode);

/**
 * @brief  Get the current state of a specific actuator.
 * @param  rack Pointer to the Rack0 instance.
 * @param  actuatorID ID of the actuator to query.
 * @retval State of the actuator (STATE_ON/STATE_OFF).
 */
bool Rack0_GetActuatorState(Rack0 *rack, uint8_t actuatorID);

/**
 * @brief  Get the current PWM duty cycle of a specific actuator.
 * @param  rack Pointer to the Rack0 instance.
 * @param  actuatorID ID of the actuator to query.
 * @retval Current PWM duty cycle (0-100%).
 */
uint8_t Rack0_GetPWMDuty(Rack0 *rack, uint8_t actuatorID);

#endif /* INC_RACK0_ACTUATOR_H_ */
