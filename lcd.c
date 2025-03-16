#include "stm32l4xx_hal.h"
#include "events.h"
void Write_SR_LCD(uint8_t temp)
{
	uint8_t i;
	uint8_t mask = 0b10000000;
	for (i = 0; i < 8; i++)
	{
		if ((temp & mask) == 0)
			GPIOB->ODR &= ~(1 << 5);
		else
			GPIOB->ODR |= (1 << 5);

		/*	Sclck */
		GPIOA->ODR &= ~(1 << 5);
		GPIOA->ODR |= (1 << 5);
		Delay(1);

		mask = mask >> 1;
	}

	/*Latch*/
	GPIOA->ODR |= (1 << 10);
	GPIOA->ODR &= ~(1 << 10);
}
void LCD_nibble_write(uint8_t temp, uint8_t s)
{

	/*writing instruction*/
	if (s == 0)
	{
		temp = temp & 0xF0;
		temp = temp | 0x02; /*RS (bit 0) = 0 for Command EN (bit1)=high */
		Write_SR_LCD(temp);

		temp = temp & 0xFD; /*RS (bit 0) = 0 for Command EN (bit1) = low*/
		Write_SR_LCD(temp);
	}

	/*writing data*/
	else if (s == 1)
	{
		temp = temp & 0xF0;
		temp = temp | 0x03; /*RS(bit 0)=1 for data EN (bit1) = high*/
		Write_SR_LCD(temp);

		temp = temp & 0xFD; /*RS(bit 0)=1 for data EN(bit1) = low*/
		Write_SR_LCD(temp);
	}
}
void Write_Instr_LCD(uint8_t code)
{
	LCD_nibble_write(code & 0xF0, 0);

	code = code << 4;
	LCD_nibble_write(code, 0);
}
void Write_Char_LCD(uint8_t code)
{
	LCD_nibble_write(code & 0xF0, 1);

	code = code << 4;
	LCD_nibble_write(code, 1);
}

void Write_String_LCD(char *temp)
{
	int i = 0;
	while (temp[i] != 0)
	{
		Write_Char_LCD(temp[i]);
		i = i + 1;
	}
}

void LCD_Init()
{
	uint32_t temp;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	/* enable GPIOB clock */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	temp = GPIOA->MODER;
	temp &= ~(0x03 << (2 * 5));
	temp |= (0x01 << (2 * 5));
	temp &= ~(0x03 << (2 * 10));
	temp |= (0x01 << (2 * 10));
	GPIOA->MODER = temp;
	temp = GPIOA->OTYPER;
	temp &= ~(0x01 << 5);
	temp &= ~(0x01 << 10);
	GPIOA->OTYPER = temp;
	temp = GPIOA->PUPDR;
	temp &= ~(0x03 << (2 * 5));
	temp &= ~(0x03 << (2 * 10));
	GPIOA->PUPDR = temp;
	temp = GPIOB->MODER;
	temp &= ~(0x03 << (2 * 5));
	temp |= (0x01 << (2 * 5));
	GPIOB->MODER = temp;
	temp = GPIOB->OTYPER;
	temp &= ~(0x01 << 5);
	GPIOB->OTYPER = temp;
	temp = GPIOB->PUPDR;
	temp &= ~(0x03 << (2 * 5));
	GPIOB->PUPDR = temp;
}