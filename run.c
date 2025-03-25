#include <stdint.h>
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

void keyPress5Callback(enum KEYPAD key)
{
	if (key != KEY_5)
		return;
	GPIOA->ODR ^= (1 << 0);

	// Write_Char_LCD(KEYPAD_CHARS[key]);
}
void keyPressCallback(enum KEYPAD key)
{
	if (key == KEY_5)
		return;
	GPIOA->ODR ^= (1 << 1);
}
void switchPressCallback(enum SWITCHS key)
{
	if (!key)
		return;
	switch (key)
	{
	case BUTTON_SWITCH2:
	{
		Write_String_LCD(" SW2 ");
		break;
	}
	case BUTTON_SWITCH3:
	{
		Write_String_LCD(" SW3 ");
		break;
	}
	case BUTTON_SWITCH4:
	{
		Write_String_LCD(" SW4 ");
		break;
	}
	case BUTTON_SWITCH5:
	{
		Write_String_LCD(" SW5 ");
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
	char *line1 = "Big Daddy";
	char *line2 = "Spring 2023";
	/*Write_Char_LCD('o');*/
	/*Write_String_LCD(line1);
	Write_Instr_LCD(0xc0); /* move to line 2*/
	// Write_String_LCD(line2);
	Events.onKeyPadPress(keyPressCallback);
	Events.onKeyPadPress(keyPress5Callback);
	// Events.onSwitchPress(switchPressCallback);

	Init_buzzer();
	Delay(1000);
	double current = date();
	char *str = doubleToString(current, 3);
	Write_String_LCD(str);

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
			//GPIOC->ODR ^= (1 << 9);
			prev_date = current;
		}
	}
}
