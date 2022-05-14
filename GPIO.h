/*
 * GPIO.h
 *
 *  Created on: 13/05/2022
 *      Author: Nelida Hernández
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "fsl_port.h"
#include "fsl_gpio.h"


#define BOARD_SW3_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW3_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW3_PIN 4U     /*!<@brief PORTA pin index: 4 */

typedef enum{GPIO_A, /*!< Definition to select GPIO A */
			 GPIO_B, /*!< Definition to select GPIO B */
			 GPIO_C, /*!< Definition to select GPIO C */
			 GPIO_D, /*!< Definition to select GPIO D */
			 GPIO_E, /*!< Definition to select GPIO E */
			 GPIO_F  /*!< Definition to select GPIO F */
			} gpio_port_name_t;

void init_sw3(void); //Inicialization for the SW3


#endif /* GPIO_H_ */
