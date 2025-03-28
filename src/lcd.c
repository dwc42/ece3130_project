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
void Clear_Display()
{
	Write_Instr_LCD(0x01);
	Write_Instr_LCD(0x02);
}
void Set_LCD(char *string)
{
	Clear_Display();
	HAL_Delay(5);
	Write_String_LCD(string);
}
void Set_Cursor(uint8_t value)
{
	if (value)
		Write_Instr_LCD(0x0E);
	else
		Write_Instr_LCD(0x0c);
}
void Set_Cursor_Blinking()
{
	Write_Instr_LCD(0x0F);
}

void Set_Display(uint8_t value)
{
	if (value)
		Write_Instr_LCD(0x0C);
	else
		Write_Instr_LCD(0x08);
}
void Set_Cursor_Write_Shift(uint8_t value)
{
	if (value)
		Write_Instr_LCD(0x14);
	else
		Write_Instr_LCD(0x10);
}
void Set_Text_Write_Shift(uint8_t value)
{
	if (value)
		Write_Instr_LCD(0x1C);
	else
		Write_Instr_LCD(0x18);
}
void LCD_Init()
{
	uint32_t temp;
	/* enable GPIOA clock */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	/* enable GPIOB clock */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	/*PB5 MOSI, PA10 /CS_7 latch , PA5 shift clock */
	/*PA5 and PA10 are outputs*/ temp = GPIOA->MODER;
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
	/* LCD controller reset sequence */
	Delay(20);
	LCD_nibble_write(0x30, 0);
	Delay(5);
	LCD_nibble_write(0x30, 0);
	Delay(1);
	LCD_nibble_write(0x30, 0);
	Delay(1);
	LCD_nibble_write(0x20, 0);
	Delay(1);

	Write_Instr_LCD(0x28); /* set 4 bit data LCD - two line display - 5x8 font*/
	Write_Instr_LCD(0x0E); /* turn on display, turn on cursor , turn off blinking */
	Write_Instr_LCD(0x01); /* clear display screen and return to home position*/
	Write_Instr_LCD(0x06); /* move cursor to right (entry mode set instruction)*/
}
