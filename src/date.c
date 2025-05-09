#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"
/**void DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable DWT
	DWT->CYCCNT = 0;								// Reset counter
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;			// Enable counter
}**/
/**
 * from stack overflow
 */
static inline uint32_t LL_SYSTICK_IsActiveCounterFlag(void)
{
	return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}
uint32_t lastTick = 0;
uint16_t ticks = 0;
void checkDate()
{
	
	if (HAL_GetTick() >= lastTick){
		lastTick = HAL_GetTick();
		return;
	}
	lastTick = HAL_GetTick();
	ticks++;
}
/**
 * from stack overflow
 */
long getCurrentMicros(void)
{
	/* Ensure COUNTFLAG is reset by reading SysTick control and status register */
	LL_SYSTICK_IsActiveCounterFlag();
	uint32_t m = HAL_GetTick();
	const uint32_t tms = SysTick->LOAD + 1;
	__IO uint32_t u = tms - SysTick->VAL;
	if (LL_SYSTICK_IsActiveCounterFlag())
	{
		m = HAL_GetTick();
		u = tms - SysTick->VAL;
	}
	//volatile long test = pow(2, 32);
	//volatile long val = (ticks) ? ticks *  test : 0;
	volatile long end = /*val +*/ m * 1000 + (u * 1000) / tms;
	return end;
}
/**int lastValue = 0;
int completeCycles = 0;
void dwt_check()
{
	int current = DWT->CYCCNT;
	int diff = current - lastValue;
	if (diff <= 0)
	{
		completeCycles++;
	}
	lastValue = current;
}
int freq = 80000000;
**/

double date()
{
	return ((double)getCurrentMicros()) / 1000.0;
}