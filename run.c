#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "events.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "keypad.h"
#include "lcd.h"
#include "buzzer.h"
#include "date.h"
#include "number.h"
void SystemClock_Config(void);

void EnableClock()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
}
void Init_LED(int i)
{
	/* enable
   GPIOB clock */
	GPIOA->MODER &= ~(0x03 << (2 * i));
	GPIOA->MODER |= (0x01 << (2 * i));
	GPIOA->OTYPER &= ~(0x01 << i);
	GPIOA->PUPDR &= ~(0x03 << (2 * i));
}
char *charToString(char c)
{
	if (c == '\0')
		return "";
	char *str = (char *)malloc(2);
	str[0] = c;
	str[1] = '\0';
	return str;
}
void keyPressCallback(enum KEYPAD key)
{
	GPIOA->ODR ^= (1 << 0);
	Set_LCD(charToString(KEYPAD_CHARS[key]));
}

void switchPressCallback(enum SWITCHS key)
{
	double current = date();
	char *str = doubleToString(current, 10);
	Set_LCD(str);
	switch (key)
	{
	case BUTTON_SWITCH2:
	{
		Write_String_LCD(" Q");
		break;
	}
	case BUTTON_SWITCH3:
	{
		Write_String_LCD(" R");
		break;
	}
	case BUTTON_SWITCH4:
	{
		Write_String_LCD(" S");
		break;
	}
	case BUTTON_SWITCH5:
	{
		Write_String_LCD(" T");
		break;
	}
	}
}
/**
 * @brief  The application entry point.
 * @retval int
 */
int run(void)
{

	EnableClock();
	Delay(500);
	Init_LED(1);
	Init_LED(0);
	LCD_Init();
	InitEvents();
	DWT_Init();
	/*Write_Char_LCD('o');*/
	/*Write_String_LCD(line1);
	Write_Instr_LCD(0xc0); /* move to line 2*/
	// Write_String_LCD(line2);
	Events.onKeyPadPress(keyPressCallback);
	Events.onSwitchPress(switchPressCallback);

	Init_buzzer();
	Delay(1000);

	double prev_date = date();
	double p = (1 / 500) * 1000;

	while (1)
	{
		// Write_Char_LCD('1');
		// Delay(1000);
		check();
		dwt_check();
		double current = date();
		if ((current - prev_date) > p)
		{
			// GPIOC->ODR ^= (1 << 9);
			prev_date = current;
		}
	}
}
