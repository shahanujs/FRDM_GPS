#include "mkl25z4.h"
#include <stdint.h>

#define SYSTEM_CLOCK_FREQ      48000000UL  // 48 Mhz
#define SYTICK_TIME_INTERVAL   1           // 1 msec
#define SYTICK_TIME_FREQ       (SYTICK_TIME_INTERVAL * 1000) // 1000 Khz
#define SYSTICK_TMR_RELOAD_VAL ((SYSTEM_CLOCK_FREQ / SYTICK_TIME_FREQ) - 1UL) // 48000 - 1

// Local function prototypes
static void gpio_init(void);
static void config_sys_clock(void);
static void SysTickConfig(void);
static void uart0_init(void);


void bsp_init(void)
{
	// Disable all global interrupts
	__disable_irq();

	// Configure System Clock
	config_sys_clock();

	// Initialize GPIO ports
	gpio_init();

	// Initialize system tick timer
	SysTickConfig();

	// Initialize UART module
	uart0_init();

	// Enable all global interrupts
	__enable_irq();
}

static void config_sys_clock(void)
{
	// MCG_C1: CLKS (bit 7-6) = 00
	MCG->C1 |= MCG_C1_CLKS(0);  // Select PLL/FLL as clock source

	// MCG_C1: IREFS (bit 2)  = 1
	MCG->C1 |= MCG_C1_IREFS(1); // Select Internal Reference clock
	                            // source for FLL

	// MCG_C4: DRST_DRS (bit 6-5) = 01
	MCG->C4 |= MCG_C4_DRST_DRS(1); // Select DCO range as Mid range
	                               // DCO generates the FLL Clock

	// MCG_C4: DMX32 (bit 7) = 1
	MCG->C4 |= MCG_C4_DMX32(1);    // Select DCO frequency as 48Mhz
}

static void gpio_init(void)
{
	SIM->SCGC5 |= (uint32_t)(1 << 10);  /* Enable clock to PORTB */
	SIM->SCGC5 |= (uint32_t)(1 << 12);  /* Enable clock to PORTD */

	PORTB->PCR[18] = (uint32_t)(0x00000100); /* PTB18 as GPIO */
	PORTB->PCR[19] = (uint32_t)(0x00000100); /* PTB19 as GPIO */
	PORTD->PCR[1]  = (uint32_t)(0x00000100); /* PTD1  as GPIO */

	GPIOB->PDDR |= (uint32_t)(1 << 18); /* PTB18 is Output */
	GPIOB->PDDR |= (uint32_t)(1 << 19); /* PTB19 is Output */
	GPIOD->PDDR |= (uint32_t)(1 << 1);  /* PTD1  is Output */

	GPIOB->PDOR |= (uint32_t)(1 << 18); /* PTB18 is driven HIGH */
	GPIOB->PDOR |= (uint32_t)(1 << 19); /* PTB18 is driven HIGH */
	GPIOD->PDOR |= (uint32_t)(1 << 1);  /* PTD1  is driven HIGH */
}

static void SysTickConfig(void)
{
  SysTick->LOAD  = (uint32_t)(SYSTICK_TMR_RELOAD_VAL);              /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
}

static void uart0_init(void)
{
	// Select MCGFLLCLK as UART0 clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	// Enable UART0 Clock
	SIM->SCGC4 |= SIM_SCGC4_UART0(1);

	// Enable PORTA clock
	SIM->SCGC5 |= SIM_SCGC5_PORTA(1);

	PORTA->PCR[1] |=  PORT_PCR_MUX(2); /* PTA1 as ALT2 (UART0) */
	PORTA->PCR[2] |=  PORT_PCR_MUX(2); /* PTA2 as ALT2 (UART0) */

	// Configure Baud Rate as 9600
	UART0->BDL = 0x38;
	UART0->BDH = 0x1;

	// Configure Serial Port as 8-N-1
	// (8 data bits, No parity and 1 stop bit)
	UART0->C1  = 0x00;

	// Configure Tx/Rx Interrupts
	UART0->C2  |= UART_C2_TIE(0);  // Tx Interrupt disabled
	UART0->C2  |= UART_C2_TCIE(0); // Tx Complete Interrupt disabled
	UART0->C2  |= UART_C2_RIE(1);	 // Rx Interrupt enabled

	// Configure Transmitter/Receiever
	UART0->C2  |= UART_C2_TE(0);	 // Tx Enabled
	UART0->C2  |= UART_C2_RE(1);	 // Rx Enabled

	// Enable UART0 Interrupt
	NVIC_EnableIRQ(UART0_IRQn);
}
