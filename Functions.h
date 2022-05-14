/*
 * Functions.h
 *
 *  Created on: 13/05/2022
 *      Author: Nelida Hernández
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include "DAC.h"
#include "DMA.h"
#include "Bits.h"
#include "NVIC.h"

#define TCD_QUEUE_SIZE      1U
void configure_transm(void); //Configuration for the transmision from DMA


#endif /* FUNCTIONS_H_ */
