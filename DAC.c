/*
 * DAC.c
 *
 *  Created on: 13/05/2022
 *      Author: Mauricio Peralta
 */

#include "DAC.h"

void init_DAC(void)
{
	//Configuration for DAC
	dac_config_t dacConfigStruct;
	DAC_GetDefaultConfig(&dacConfigStruct);
	DAC_Init(DAC0, &dacConfigStruct);
	DAC_Enable(DAC0, true);
	DAC_SetBufferReadPointer(DAC0, DAC_INDEX);
}
