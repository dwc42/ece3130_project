#include "buzzer.h"
#include <time.h>
#include <stdint.h>


double date() 
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); // Get the current time
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}
void Init_buzzer()
{
    
    uint32_t temp;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; /* enable GPIOC clock*/
    temp = GPIOC->MODER;
    temp &= ~(0x03<<(2*9));
    temp|=(0x01<<(2*9));
    GPIOC->MODER = temp;
    temp=GPIOC->OTYPER;
    temp &=~(0x01<<9);
    GPIOC->OTYPER=temp;
    temp=GPIOC->PUPDR;
    temp&=~(0x03<<(2*9));
    GPIOC->PUPDR=temp;
}

