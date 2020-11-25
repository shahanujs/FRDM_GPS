#include "mkl25z4.h"
#include <stdint.h>
#include "isr.h"

// Time tick counters
uint16_t base_tick;
uint8_t tick_50msec;
uint8_t tick_100msec;
uint8_t tick_500msec;

// Timer tick flag
volatile uint8_t flag_50msec;
volatile uint8_t flag_100msec;
volatile uint8_t flag_500msec;
volatile uint8_t flag_1sec;

void SysTick_Handler(void)
{
	// 1msec base tick
	++base_tick;

	// Generate 50msec time
	if (base_tick >= 50U)
	{
		base_tick = 0U;
		flag_50msec = 1U;

		++tick_50msec;
	}

	// Generate 100msec time
	if (tick_50msec >= 2U)
	{
		tick_50msec = 0U;
		flag_100msec = 1U;

		++tick_100msec;
	}

	// Generate 500msec time
	if (tick_100msec >= 5U)
	{
		tick_100msec = 0U;
		flag_500msec = 1U;

		++tick_500msec;
	}

	// Generate 1sec time
	if (tick_500msec >= 2U)
	{
		tick_500msec = 0U;
		flag_1sec = 1U;
	}
}

// UART0 status and error Interrupt
void UART0_IRQHandler(void)
{
	// Get receive input
	if ((UART0->S1 & UART_S1_RDRF_MASK))
	{
		UART0->D;
	}
}

