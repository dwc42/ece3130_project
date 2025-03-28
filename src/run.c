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
double time = 0;

double Frequencies[16] = {10000, 1174, 1318, 1396,
						  392, 440, 493.88, 523.25,
						  1000, 1200, 1400,
						  1600, 1800, 2000, 2500};

/*double Frequencies[16] = {1, 2, 3, 4,
						  5, 6, 7, 8,
						  9, 10, 11,
						  12, 13, 14, 15};*/
double peroid = 0.0;
void keyPressCallback(enum KEYPAD key)
{
	peroid = 1000.0 / ((double)Frequencies[key]);
	// GPIOA->ODR |= (1 << 1);
	Write_Char_LCD(KEYPAD_CHARS[key]);
}
void keyReleaseCallback(enum KEYPAD key)
{
	peroid = 0;
	// GPIOA->ODR &= ~(1 << 1);
	Write_Char_LCD(KEYPAD_CHARS[key]);
}
void switchPressCallback(enum SWITCHS key)
{
	double current = date();
	char *str = doubleToString(time, 10);
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
	/*DWT_Init();*/
	/*Write_Char_LCD('o');*/
	/*Write_String_LCD(line1);
	Write_Instr_LCD(0xc0); /* move to line 2*/
	// Write_String_LCD(line2);
	Events.onKeyPadPress(keyPressCallback);
	Events.onKeyPadRelease(keyReleaseCallback);
	Events.onSwitchPress(switchPressCallback);

	Init_buzzer();
	double prev_date = date();
	while (1)
	{
		double old = date();
		HAL_Delay(2);
		double test = date();
		
		HAL_Delay(2000);
		char *str = doubleToString(test- old, 2);
		Set_LCD(str);
		check();
		double current = date();
		if (!peroid)
			continue;
		if (((current - prev_date) > peroid))
		{
			GPIOC->ODR ^= (1 << 9);
			//GPIOA->ODR ^= 1 << 1;
			prev_date = current;
		}
	}
}
