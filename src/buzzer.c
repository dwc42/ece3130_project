#include "buzzer.h"
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <float.h>
#include "list.h"
#include "main.h"
#include <float.h>
#include "date.h"
#include "stm32l4xx_hal_rcc.h"
#include "stm32l4xx_hal_tim.h"
#include <stdint.h>
int *frequency_list; // List of frequencies, initialized to NULL

// TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htimEXT[4] = {{0}, {0}, {0}, {0}};
/**
 *
 * @brief Initialize the buzzer on PC9 using TIM3 in PWM mode
 * @note This function configures the GPIO pin and the timer for PWM output. chat gpt suggested this code for buzzer( as not covered in the slides)
 */
// void Init_buzzer(void)
// {
//     // Initialize the frequency list if it hasn't been done yet
//     frequency_list = malloc(sizeof(INT32_MAX)); // allocate space for one integer and INT32_MAX terminator
//     frequency_list[0] = INT32_MAX;              // set the terminator

//     // Enable TIM3 and GPIOC clocks
//     __HAL_RCC_TIM3_CLK_ENABLE();  // Enable TIM3 clock
//     __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC clock

//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     // Configure PC9 (TIM3 Channel 4) as Alternate Function Push-Pull
//     GPIO_InitStruct.Pin = GPIO_PIN_9;
//     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;       // Alternate Function Push-Pull
//     GPIO_InitStruct.Pull = GPIO_NOPULL;           // No pull-up or pull-down
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // High speed
//     GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;    // Alternate function for TIM3
//     HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//     // Configure TIM3 for PWM mode

//     htim3.Instance = TIM3;
//     htim3.Init.Prescaler = 72 - 1; // Assuming 72MHz system clock, prescale to 1MHz
//     htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
//     htim3.Init.Period = 2000 - 1; // 1MHz / 1000 = 1kHz PWM frequency
//     htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//     htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

//     TIM_OC_InitTypeDef sConfigOC = {0};
//     sConfigOC.OCMode = TIM_OCMODE_PWM1;
//     sConfigOC.Pulse = 500; // 50% Duty Cycle
//     sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//     sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
// }
void enable_tim_clocks()
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_TIM5_CLK_ENABLE();
}
struct timer
{
    GPIO_TypeDef *GPIO;
    uint16_t GPIO_PIN;
    uint32_t GPIO_AF2_TIM;
    uint64_t TIM_OCMODE_PWM;
    uint32_t TIM_CHANNEL;
    TIM_TypeDef *TIM;
};
struct timer timers[4] = {
    {GPIOC, GPIO_PIN_9, GPIO_AF2_TIM3, TIM_OCMODE_PWM1, TIM_CHANNEL_4, TIM3},
    {GPIOB, GPIO_PIN_6, GPIO_AF2_TIM4, TIM_OCMODE_PWM1, TIM_CHANNEL_1, TIM4},
    {GPIOB, GPIO_PIN_1, GPIO_AF2_TIM2, TIM_OCMODE_PWM1, TIM_CHANNEL_2, TIM2},
    {GPIOC, GPIO_PIN_2, GPIO_AF2_TIM4, TIM_OCMODE_PWM1, TIM_CHANNEL_4, TIM4}};
void Init_buzzerEXT(uint8_t timer)
{
    // Initialize the frequency list if it hasn't been done yet
    frequency_list = malloc(sizeof(INT32_MAX)); // allocate space for one integer and INT32_MAX terminator
    frequency_list[0] = INT32_MAX;              // set the terminator

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure PC9 (TIM3 Channel 4) as Alternate Function Push-Pull
    GPIO_InitStruct.Pin = timers[timer].GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                 // Alternate Function Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;                     // No pull-up or pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;           // High speed
    GPIO_InitStruct.Alternate = timers[timer].GPIO_AF2_TIM; // Alternate function for TIM3
    HAL_GPIO_Init(timers[timer].GPIO, &GPIO_InitStruct);

    // Configure TIM3 for PWM mode

    htimEXT[timer].Instance = timers[timer].TIM;
    htimEXT[timer].Init.Prescaler = 72 - 1; // Assuming 72MHz system clock, prescale to 1MHz
    htimEXT[timer].Init.CounterMode = TIM_COUNTERMODE_UP;
    htimEXT[timer].Init.Period = 2000 - 1; // 1MHz / 1000 = 1kHz PWM frequency
    htimEXT[timer].Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htimEXT[timer].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = timers[timer].TIM_OCMODE_PWM;
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
int lastSwitchTime = 13;
void CheckFrequency()
{

    if (date() - lastDateFrequency <= lastSwitchTime)
        return;
    int smallest = 0;
    uint8_t i = 0;
    for (; frequency_list[i] != INT32_MAX; i++)
        if (frequency_list[i] < smallest)
            smallest = frequency_list[i];
    lastSwitchTime = 20 - smallest / 100;
    lastDateFrequency = date();
    if (frequency_list[0] == INT32_MAX)
    {
        SetFrequency(0, 1);
        return;
    }
    // Update the last date frequency to now, to avoid spamming the buzzer
    if (frequencyIndex > 3 || frequency_list[frequencyIndex] == INT32_MAX)
    {
        frequencyIndex = 0;
    }
    // Get the frequency from the list
    int freq = frequency_list[frequencyIndex];

    // Set the frequency on the buzzer
    SetFrequency((double)freq, 1);
    frequencyIndex++;
    // Only play the first frequency in the list for now, can be extended to play multiple in sequence if needed
}
/**
 * @brief Set the frequency of the buzzer
 * @param freq The desired frequency in Hz
 * @note gpt suggested this code for buzzer
 */
void SetFrequency(double freq, uint8_t timer)
{
    int freqInt = (int)freq;
    if (currentFrequency == freqInt)
        return;
    currentFrequency = freqInt;
    if (freq <= 0)
    {
        // Stop the timer if frequency is 0
        HAL_TIM_PWM_Stop(&htimEXT[timer], timers[timer].TIM_CHANNEL);
        return;
    }

    uint32_t timerClock = 72000000;                                // Assuming system clock is 72 MHz
    uint32_t prescaler = 72 - 1;                                   // Prescale to 1 MHz timer clock
    uint32_t period = timerClock / ((freq + 1) * (prescaler + 1)); // <-- owens/blog chat gpt's -> (timerClock / (prescaler + 1) / freq) - 1;

    // Update the timer configuration
    htimEXT[timer].Init.Prescaler = prescaler;
    htimEXT[timer].Init.Period = period;

    if (HAL_TIM_Base_Init(&htimEXT[timer]) != HAL_OK)
    {
        // Initialization error
        while (1)
            ;
    }

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = timers[timer].TIM_OCMODE_PWM;
    sConfigOC.Pulse = period / 2; // 50% duty cycle
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htimEXT[timer], &sConfigOC, timers[timer].TIM_CHANNEL) != HAL_OK)
    {
        // Configuration error
        while (1)
            ;
    }
    //__HAL_TIM_MOE_ENABLE(&htimEXT[timer]);
    // Restart the timer with the new configuration
    if (HAL_TIM_PWM_Start(&htimEXT[timer], timers[timer].TIM_CHANNEL) != HAL_OK)
    {
        // Start error
        while (1)
            ;
    }
}