#include "buzzer.h"
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <time.h>
#include <float.h>
#include <list.h>
#include "main.h"
#include <float.h>
#include "date.h"

int *frequency_list; // List of frequencies, initialized to NULL

TIM_HandleTypeDef htim3;
/**
 *
 * @brief Initialize the buzzer on PC9 using TIM3 in PWM mode
 * @note This function configures the GPIO pin and the timer for PWM output. chat gpt suggested this code for buzzer( as not covered in the slides)
 */
void Init_buzzer(void)
{
    // Initialize the frequency list if it hasn't been done yet
    frequency_list = malloc(sizeof(INT32_MAX)); // allocate space for one integer and INT32_MAX terminator
    frequency_list[0] = INT32_MAX;              // set the terminator

    // Enable TIM3 and GPIOC clocks
    __HAL_RCC_TIM3_CLK_ENABLE();  // Enable TIM3 clock
    __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC clock

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure PC9 (TIM3 Channel 4) as Alternate Function Push-Pull
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;       // Alternate Function Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;           // No pull-up or pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // High speed
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;    // Alternate function for TIM3
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Configure TIM3 for PWM mode

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 72 - 1; // Assuming 72MHz system clock, prescale to 1MHz
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 2000 - 1; // 1MHz / 1000 = 1kHz PWM frequency
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 500; // 50% Duty Cycle
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
}
void AddFrequency(double freq)
{
    pushInteger(&frequency_list, (int)freq);
}
void RemoveFrequency(double freq)
{
    removeFromIntegers(&frequency_list, (int)freq, 2);
}
double lastDateFrequency = 0.0;
int frequencyIndex = 0;
int currentFrequency = 0;
void CheckFrequency()
{
    if (date() - lastDateFrequency <= 2)
        return;
		lastDateFrequency = date();
		if (frequency_list[0] == INT32_MAX)
		{
			SetFrequency(0);
			return;
		}
     // Update the last date frequency to now, to avoid spamming the buzzer
    if (frequencyIndex >= 4 || frequency_list[frequencyIndex] == INT32_MAX)
    {
        frequencyIndex = 0;
    }
    // Get the frequency from the list
    int freq = frequency_list[frequencyIndex];

    // Set the frequency on the buzzer
    SetFrequency((double)freq);
		frequencyIndex++;
    // Only play the first frequency in the list for now, can be extended to play multiple in sequence if needed
}
/**
 * @brief Set the frequency of the buzzer
 * @param freq The desired frequency in Hz
 * @note gpt suggested this code for buzzer
 */
void SetFrequency(double freq)
{	
		int freqInt = (int) freq;
		if (currentFrequency == freqInt) return;
		currentFrequency = freqInt;
    if (freq <= 0)
    {
        // Stop the timer if frequency is 0
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
        return;
    }

    uint32_t timerClock = 72000000;                                // Assuming system clock is 72 MHz
    uint32_t prescaler = 72 - 1;                                   // Prescale to 1 MHz timer clock
    uint32_t period = timerClock / ((freq + 1) * (prescaler + 1)); // <-- owens/blog chat gpt's -> (timerClock / (prescaler + 1) / freq) - 1;

    // Update the timer configuration
    htim3.Init.Prescaler = prescaler;
    htim3.Init.Period = period;

    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        // Initialization error
        while (1)
            ;
    }

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = period / 2; // 50% duty cycle
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        // Configuration error
        while (1)
            ;
    }

    // Restart the timer with the new configuration
    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4) != HAL_OK)
    {
        // Start error
        while (1)
            ;
    }
}