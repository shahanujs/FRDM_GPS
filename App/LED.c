/**
 *  Created on: Nov 4, 2020
 *  Author: Anuj
 */

#include "mkl25z4.h"
#include <stdint.h>

#define toggle_red_LED() 	{GPIOB->PTOR |= ((uint32_t)(1<<18));}
#define toggle_green_LED() 	{GPIOB->PTOR |= ((uint32_t)(1<<19));}
#define toggle_blue_LED() 	{GPIOD->PTOR |= ((uint32_t)(1<<1));}

void control_LEDs(void)
{
	toggle_red_LED();
}
