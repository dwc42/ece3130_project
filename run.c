#include <stdint.h>
#include <stdlib.h>
#include "events.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "keypad.h"
void SystemClock_Config(void);

void EnableClock()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
}
void Init_LED(int i)
{
	/* enable
   GPIOB clock */
	GPIOA->MODER &= ~(0x03 << (2 * i));
	GPIOA->MODER |= (0x01 << (2 * i));
	GPIOA->OTYPER &= ~(0x01 << i);
	GPIOA->PUPDR &= ~(0x03 << (2 * i));
}

void keyPressCallback(enum KEYPAD key)
{
	if (!key)
		return;
	GPIOA->ODR ^= (1 << 1);
}
/**
 * @brief  The application entry point.
 * @retval int
 */
int run(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick.
	 */

	/* Configure the system clock */
	SystemClock_Config();
	int LED = 1;
	EnableClock();
	Init_LED(LED);
	InitEvents();
	Events.onKeyPadRelease(keyPressCallback);
	while (1)
	{
		check();
	}

	// GPIOA->ODR ^=(1<<1);
	// Delay(1000);
	
}
