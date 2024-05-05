/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void) {
    // Initialize PA12
    IOMUX->SECCFG.PINCM[PA12INDEX] = 0x00040081;

    // Initialize PA31
    IOMUX->SECCFG.PINCM[PA31INDEX] = 0x00040081;

    // Initialize PA24
    IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00040081;

    // Initialize PA28
    IOMUX->SECCFG.PINCM[PA28INDEX] = 0x00040081;

    GPIOA->DOE31_0 |= ((1 << 12) + (1 << 31) + (1 << 24) + (1 << 28)); //create mask
}

// return current state of switches
uint32_t Switch_In(void){
    // write this
    uint32_t test = GPIOA->DIN31_0;
    return test;
}
