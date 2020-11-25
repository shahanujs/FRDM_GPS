#include "BSPInit.h"
#include <stdint.h>
#include "LED.h"
#include "isr.h"

int main()
{
	// Initialize your Board Support Package
	bsp_init();

	for (;;)
	{
		// 500msec task
		if (flag_500msec)
		{
			// control/blink LED's every 500msec
			control_LEDs();
		  flag_500msec = 0U;
		}
	}
}
