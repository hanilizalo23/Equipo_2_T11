/*
 * GPIO.c
 *
 *  Created on: 13/05/2022
 *      Author: Nelida Hernández
 */

#include "GPIO.h"

static void (*gpio_A_callback)(void) = 0;

void GPIO_callback_init(gpio_port_name_t port_name,void (*handler)(void))
{
	if(GPIO_A == port_name)
	{
		gpio_A_callback = handler; //Set GPIO A Callback
	}
}

void init_sw3(void)
{
    CLOCK_EnableClock(kCLOCK_PortA);
    gpio_pin_config_t SW3_config = {
          .pinDirection = kGPIO_DigitalInput,
          .outputLogic = 0U
      };

    /* Initialize GPIO functionality on pin PTA4 (pin 38)  */
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_PIN, &SW3_config);
    const port_pin_config_t SW3 = {/* Internal pull-up/down resistor is disabled */
                                    kPORT_PullDisable,
                                    /* Fast slew rate is configured */
                                    kPORT_FastSlewRate,
                                    /* Passive filter is disabled */
                                    kPORT_PassiveFilterEnable,
                                    /* Open drain is disabled */
                                    kPORT_OpenDrainDisable,
                                    /* Low drive strength is configured */
                                    kPORT_LowDriveStrength,
                                    /* Pin is configured as PTA4 */
                                    kPORT_MuxAsGpio,
                                    /* Pin Control Register fields [15:0] are not locked */
                                    kPORT_UnlockRegister};
     /* PORTA4 (pin 38) is configured as PTA4 */
     PORT_SetPinConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, &SW3);
     /* Interrupt configuration on PORTA4 (pin 38): Interrupt on falling edge */
     PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, kPORT_InterruptFallingEdge);
}
