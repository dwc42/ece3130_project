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
TIM_HandleTypeDef htim3;
void Init_buzzer(void)
{
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

    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
        // Initialization error
        while (1)
            ;
    }

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 500; // 50% Duty Cycle
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        // Configuration error
        while (1)
            ;
    }
}
/**
 * @brief Set the frequency of the buzzer
 * @param freq The desired frequency in Hz
 */
void SetFrequency(double freq)
{
    if (freq <= 0)
    {
        // Stop the timer if frequency is 0
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
        return;
    }

    uint32_t timerClock = 72000000; // Assuming system clock is 72 MHz
    uint32_t prescaler = 72 - 1;    // Prescale to 1 MHz timer clock
    uint32_t period = (timerClock / (prescaler + 1) / freq) - 1;

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