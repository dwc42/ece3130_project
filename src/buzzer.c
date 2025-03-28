#include "buzzer.h"
#include <time.h>
#include <stdint.h>

#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <time.h>

void Init_buzzer()
{

    uint32_t temp;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; /* enable GPIOC clock*/
    temp = GPIOC->MODER;
    temp &= ~(0x03 << (2 * 9));
    temp |= (0x01 << (2 * 9));
    GPIOC->MODER = temp;
    temp = GPIOC->OTYPER;
    temp &= ~(0x01 << 9);
    GPIOC->OTYPER = temp;
    temp = GPIOC->PUPDR;
    temp &= ~(0x03 << (2 * 9));
    GPIOC->PUPDR = temp;
}
