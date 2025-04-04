#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "events.h"
#include "main.h"
#include "stm32l4xx_hal.h"
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

double Frequencies[16] = {33, 37, 169, 175, 196, 220, 246.9, 261.6, 294, 330, 349, 392, 440, 494, 523};

/*double Frequencies[16] = {1, 2, 3, 4,
						  5, 6, 7, 8,
						  9, 10, 11,
						  12, 13, 14, 15};*/
double peroid = 0.0;
void keyPressCallback(enum KEYPAD key)
{
	AddFrequency(Frequencies[key]);
	//Write_Char_LCD(KEYPAD_CHARS[key]);
}
void keyReleaseCallback(enum KEYPAD key)
{
	RemoveFrequency(Frequencies[key]);
	// GPIOA->ODR &= ~(1 << 1);
	//Write_Char_LCD(KEYPAD_CHARS[key]);
}
double average = 0;
void switchPressCallback(enum SWITCHS key)
{
	char *str = doubleToString(average, 3);
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

double ticksArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * @brief  The application entry point.
 * @retval int
 */
int run(void)
{

	EnableClock();
	Init_LED(1);
	Init_LED(0);
	LCD_Init();
	InitEvents();
	/*DWT_Init();*/
	/*Write_Char_LCD('o');*/
	/*Write_String_LCD(line1);
	Write_Instr_LCD(0xc0);
		move to line */
	// Write_String_LCD(line2);
	Events.onKeyPadPress(keyPressCallback);
	Events.onKeyPadRelease(keyReleaseCallback);
	Events.onSwitchPress(switchPressCallback);

	Init_buzzer();
	double prev_date = date();
	double lastTickDate = date();
	double lastPrint = date();
	Write_String_LCD("THIS BOARD SUCKS BALLS! IT NEEDS TO BE THROWN IN THE TRASH!     ");
	while (1)
	{
		CheckFrequency();
		check();
		checkLCDWrites();
		double current = date();
		double tickTime = date() - lastTickDate;
		if (peroid)
		{
			if (((current - prev_date) > peroid))
			{
				GPIOC->ODR ^= (1 << 9);
				prev_date = current;
			}
		}

		double total = 0;
		for (int i = 0; i < 10; i++)
		{
			total += ticksArray[i];
			if (i >= 9)
				continue;
			ticksArray[i + 1] = ticksArray[i];
		}
		ticksArray[0] = tickTime;
		average = total / 10;
		/*if (date() - lastPrint > 1000)
		{
			char *str = doubleToString(average, 3);
			Set_LCD(str);
			lastPrint = date();
		}*/
		lastTickDate = date();
	}
}
