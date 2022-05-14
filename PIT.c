/*
 * PIT.c
 *
 *  Created on: 13/05/2022
 *      Author: Nelida Hernández
 */

#include "PIT.h"

static void (*PIT_0_callback)(void) = 0;
static void (*PIT_1_callback)(void) = 0;

boolean_t flag_delay;

void PIT0_IRQHandler(void)
{
	if(PIT_0_callback)
	{
		PIT_0_callback();
	}
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
}

void PIT1_IRQHandler(void)
{
	if(PIT_1_callback)
	{
		PIT_1_callback();
	}
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
}

void PIT2_IRQHandler(void)
{
	flag_delay = TRUE;
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
}

void PIT_callback_init(void *handler, PIT_chn_t PIT_chnl)
{
	if(PIT0 == PIT_chnl)
	{
		PIT_0_callback = handler;
	}
	else
	{
		PIT_1_callback = handler;
	}
}

boolean_t flagDelay(void)
{
	return flag_delay;
}

void clearFlagDelay(void)
{
	flag_delay = FALSE;
}

void configureTimer(void)
{
	pit_config_t pitConfig;
	PIT_Init(PIT, &pitConfig);

	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(USEC500K, PIT_SOURCE_CLOCK));

	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT0_IRQn);
	PIT_StartTimer(PIT, kPIT_Chnl_0);
}
