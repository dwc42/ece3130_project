#include <stdio.h>
#include "stm32l4xx_hal.h"
void DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable DWT
	DWT->CYCCNT = 0;								// Reset counter
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;			// Enable counter
}
int lastValue = 0;
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
double date()
{
	int current = DWT->CYCCNT;

	int peroid = 1 / freq;
	double time = ((double)current / (double)freq) + (double)completeCycles * peroid;
	return time;
}