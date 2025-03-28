#include "stm32l4xx_hal.h"
#include "events.h"
void Keypad_Init()
{
	uint32_t temp = 0x00000000;
	/* enable GPIOB clock */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	/*configure input*/
	/* row0 to 3 are PB11, PB10, PB9, PB8 */
	temp = GPIOB->MODER;
	temp &= ~(0x03 << (2 * 11));
	temp &= ~(0x03 << (2 * 10));
	temp &= ~(0x03 << (2 * 9));
	temp &= ~(0x03 << (2 * 8));
	GPIOB->MODER = temp;

	temp = GPIOB->OTYPER;
	temp &= ~(0x01 << 11);
	temp &= ~(0x01 << 10);
	temp &= ~(0x01 << 9);
	temp &= ~(0x01 << 8);
	GPIOB->OTYPER = temp;
	temp = GPIOB->PUPDR;
	temp &= ~(0x03 << (2 * 11));
	temp &= ~(0x03 << (2 * 10));
	temp &= ~(0x03 << (2 * 9));
	temp &= ~(0x03 << (2 * 8));
	GPIOB->PUPDR = temp;

	/* Col 0 to 3 are PB1, PB2, PB3, PB4*/
	/*configure output*/ temp = GPIOB->MODER;
	temp &= ~(0x03 << (2 * 1));
	temp |= (0x01 << (2 * 1));
	temp &= ~(0x03 << (2 * 2));
	temp |= (0x01 << (2 * 2));
	temp &= ~(0x03 << (2 * 3));
	temp |= (0x01 << (2 * 3));
	temp &= ~(0x03 << (2 * 4));
	temp |= (0x01 << (2 * 4));
	GPIOB->MODER = temp;

	temp = GPIOB->OTYPER;
	temp &= ~(0x01 << 1);
	temp &= ~(0x01 << 2);
	temp &= ~(0x01 << 3);
	temp &= ~(0x01 << 4);
	GPIOB->OTYPER = temp;

	temp = GPIOB->PUPDR;
	temp &= ~(0x03 << (2 * 1));
	temp &= ~(0x03 << (2 * 2));
	temp &= ~(0x03 << (2 * 3));
	temp &= ~(0x03 << (2 * 4));
	GPIOB->PUPDR = temp;
}

uint8_t Read_Keypad()
{
	uint8_t a;
	/*set all columns high and wait until a putton is pressed*/ GPIOB->ODR |= (1 << 1);
	GPIOB->ODR |= (1 << 2);
	GPIOB->ODR |= (1 << 3);
	GPIOB->ODR |= (1 << 4);

	while ((GPIOB->IDR & (0x1 << 8)) == 0 && (GPIOB->IDR & (0x1 << 9)) == 0 && (GPIOB->IDR & (0x1 << 10)) == 0 && (GPIOB->IDR & (0x1 << 10)) == 0)
	{
	}

	Delay(25); /*debouncing*/
	/*scanning */ while (1)
	{
		GPIOB->ODR &= ~(1 << 1);
		GPIOB->ODR &= ~(1 << 2);
		GPIOB->ODR &= ~(1 << 3);
		GPIOB->ODR &= ~(1 << 4);

		GPIOB->ODR |= (1 << 1);
		if ((GPIOB->IDR & (0x1 << 11)) != 0)
		{
			a = 1;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 10)) != 0)
		{
			a = 4;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 9)) != 0)
		{
			a = 7;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 8)) != 0)
		{
			a = 14;
			break;
		}
		GPIOB->ODR &= ~(1 << 1);
		Delay(2);
		GPIOB->ODR |= (1 << 2);
		Delay(2);
		if ((GPIOB->IDR & (0x1 << 11)) != 0)

		{
			a = 2;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 10)) != 0)
		{
			a = 5;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 9)) != 0)
		{
			a = 8;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 8)) != 0)
		{
			a = 0;
			break;
		}
		GPIOB->ODR &= ~(1 << 2);
		Delay(2);
		GPIOB->ODR |= (1 << 3);
		Delay(2);

		if ((GPIOB->IDR & (0x1 << 11)) != 0)
		{
			a = 3;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 10)) != 0)
		{
			a = 6;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 9)) != 0)
		{
			a = 9;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 8)) != 0)
		{
			a = 15;
			break;
		}
		/* Scan Col 3 */ GPIOB->ODR &= ~(1 << 3);
		Delay(2);
		GPIOB->ODR |= (1 << 4);
		Delay(2);

		if ((GPIOB->IDR & (0x1 << 11)) != 0)
		{
			a = 10;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 10)) != 0)
		{
			a = 11;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 9)) != 0)
		{
			a = 12;
			break;
		}
		if ((GPIOB->IDR & (0x1 << 8)) != 0)
		{
			a = 13;
			break;
		}
	}
	GPIOB->ODR |= (1 << 1);
	Delay(2);
	GPIOB->ODR |= (1 << 2);
	Delay(2);
	GPIOB->ODR |= (1 << 3);
	Delay(2);
	GPIOB->ODR |= (1 << 4);
	Delay(2);
	while (!(((GPIOB->IDR & (0x1 << 8)) == 0) && ((GPIOB->IDR & (0x1 << 9)) == 0) && ((GPIOB->IDR & (0x1 << 10)) == 0) && ((GPIOB->IDR & (0x1 << 11)) == 0)))
	{
	}
	Delay(25);
	return a;
}
