/*
 * PIT.h
 *
 *  Created on: 13/05/2022
 *      Author: Nelida Hernández
 */

#ifndef PIT_H_
#define PIT_H_


#include "fsl_pit.h"
#include "Bits.h"
#include "fsl_port.h"

#define F100K 500
#define F200K 250
#define F500K 100

#define MSTOCOUNTINIT 1000
#define ADC_MS 10
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define DELAY_MS  500

typedef enum PITCHANNEL
{
	PIT0,
	PIT1
} PIT_chn_t;

void PIT_callback_init(void *handler, PIT_chn_t PIT_chnl); //Initialization for PIT
boolean_t flagDelay(void); //Delay for flag
void clearFlagDelay(void); //Cleaning flag
void configureTimer(void); //Configuration for the delay needed

#endif /* PIT_H_ */
