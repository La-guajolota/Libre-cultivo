/*
 * rack0_actuator.c
 *
 *  Created on: Nov 16, 2024
 *      Author: Adrián Silva Palafox
 *      Company: Fourier Embeds | Libre Cultivo
 *
 *  Description:
 *      Implementation file for managing actuators (dose pumps, fans, supply pumps, and grow LEDs)
 *      in the Rack 0 actuator system.
 */

#include "rack0_actuator.h"

/******************************************************************************
 * SECTION 1: PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * @brief  Validate the ID of an actuator.
 * @param  rack Pointer to the Rack0 instance.
 * @param  actuatorID ID to validate.
 * @retval true if valid, false otherwise.
 */
static bool Rack0_ValidateID(Rack0 *rack, uint8_t actuatorID) {
    return (actuatorID < MAX_ACTUATORS);
}

/******************************************************************************
 * SECTION 2: PUBLIC FUNCTIONS
 *****************************************************************************/

void Rack0_Init(Rack0 *rack) {
    for (uint8_t i = 0; i < MAX_ACTUATORS; i++) {
        rack->actuators[i].ID = i;           // Assign unique ID
        rack->actuators[i].pwmDuty = 0;      // Default PWM duty (0%)
        rack->actuators[i].state = STATE_OFF; // Default state (OFF)
        rack->actuators[i].mode = MODE_LOW;  // Default mode
    }
    rack->activeCount = 0;                   // No active actuators initially
}

bool Rack0_SetActuatorState(Rack0 *rack, uint8_t actuatorID, bool state) {
    if (!Rack0_ValidateID(rack, actuatorID)) {
        return false; // Invalid ID
    }

    rack->actuators[actuatorID].state = state;

    // Update active count
    if (state == STATE_ON) {
        rack->activeCount++;
    } else if (rack->activeCount > 0) {
        rack->activeCount--;
    }

    return true;
}

bool Rack0_SetPWMDuty(Rack0 *rack, uint8_t actuatorID, uint8_t pwmDuty) {
    if (!Rack0_ValidateID(rack, actuatorID) || pwmDuty > 100) {
        return false; // Invalid ID or PWM value
    }

    rack->actuators[actuatorID].pwmDuty = pwmDuty;
    return true;
}

bool Rack0_SetMode(Rack0 *rack, uint8_t actuatorID, ActuatorMode mode) {
    if (!Rack0_ValidateID(rack, actuatorID) || mode > MODE_HIGH) {
        return false; // Invalid ID or mode
    }

    rack->actuators[actuatorID].mode = mode;
    return true;
}

bool Rack0_GetActuatorState(Rack0 *rack, uint8_t actuatorID) {
    if (!Rack0_ValidateID(rack, actuatorID)) {
        return STATE_OFF; // Return default state for invalid ID
    }

    return rack->actuators[actuatorID].state;
}

uint8_t Rack0_GetPWMDuty(Rack0 *rack, uint8_t actuatorID) {
    if (!Rack0_ValidateID(rack, actuatorID)) {
        return 0; // Return default duty for invalid ID
    }

    return rack->actuators[actuatorID].pwmDuty;
}



