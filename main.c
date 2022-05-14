/*
 *
 *  Created on: 13/05/2022
 *      Author: Nelida Hernández & Mauricio Peralta
 */

#include <stdio.h>
#include "DAC.h"
#include "PIT.h"
#include "Functions.h"
#include "GPIO.h"

int main(void) {

	init_DAC();
	push_buttons_init();

    while(1)
    {
    	if(!GPIO_read_pin(GPIO_A, bit_4))
    	{
    		if(flag_a == 0)
    		{
    			memcpy(var_array, sin_values, 200*sizeof(uint16_t)); //Shouldn't use, just for practice
    			flag_a = 1;
    		}
    		if(flag_a == 1)
    		{
    			memcpy(var_array, triangle_values, 200*sizeof(uint16_t)); //Shouldn't use, just for practice
    			flag_a = 2;
    		}
    		if(flag_a == 2)
    		{
    			memcpy(var_array, sawtooth_values, 200*sizeof(uint16_t)); //Shouldn't use, just for practice
    			flag_a = 0;
    		}
    	}

    	if(!GPIO_read_pin(GPIO_C, bit_10))
    	{
    		if(flag_f == 0)
    		{
    			var_freq = F100K;
    			flag_f = 1;
    		}
    		if(flag_f == 1)
    		{
    			var_freq = F200K;
    			flag_a = 2;
    		}
    		if(flag_f == 2)
    		{
    			var_freq = F500K;
    			flag_a = 0;
    		}
    	}

    	configureTimer();
    	configure_transm();
    }
    return 0;
}
