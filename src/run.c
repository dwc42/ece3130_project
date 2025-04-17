#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "events.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "lcd.h"
#include "buzzer.h"
#include "date.h"
#include <string.h>
void SystemClock_Config(void);

void EnableClock()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
}
// void Init_LED(int i)
// {
// 	/* enable
//    GPIOB clock */
// 	GPIOA->MODER &= ~(0x03 << (2 * i));
// 	GPIOA->MODER |= (0x01 << (2 * i));
// 	GPIOA->OTYPER &= ~(0x01 << i);
// 	GPIOA->PUPDR &= ~(0x03 << (2 * i));
// }
// char *charToString(char c)
// {
// 	if (c == '\0')
// 		return "";
// 	char *str = (char *)malloc(2);
// 	str[0] = c;
// 	str[1] = '\0';
// 	return str;
// }

int modeCycle = 0;
uint8_t presetIndex = 0;

char switch_Menu[4][4];
uint8_t compareStrings(char *string1, char *string2)
{
	for (int i = 0; string1[i] != '\0'; i++)
	{
		if (string2[i] == '\0')
			return 0;
		if (string1[i] != string2[i])
			return 0;
	}
	return 1;
}
void update_SW_Menu()
{
	char sector4New[4];
	char sector5New[4];
	char sector6New[4];
	char sector7New[4];
	switch (modeCycle)
	{
	case 0:
	{
		if (presetIndex == 0)
		{
			strcpy(sector4New, "3&4");
			strcpy(sector7New, "M#0");
		}
		else if (presetIndex == 1)
		{

			strcpy(sector4New, "1&2");
		}
		else if (presetIndex == 2)
		{
			strcpy(sector4New, "5&6");
		}

		break;
	}
	case 1:
	{
		strcpy(sector7New, "M#1");
		break;
	}
	case 2:
	{
		strcpy(sector7New, "M#2");
		break;
	}
	}
	if (sector4New[0] && !compareStrings(sector4New, switch_Menu[0]))
	{
		strcpy(switch_Menu[0], sector4New);
		Write_String_Sector_LCD(4, sector4New);
	}
	
	if(sector7New[0] && !compareStrings(sector7New, switch_Menu[3]))
	{
		strcpy(switch_Menu[3], sector7New);
		Write_String_Sector_LCD(7, sector7New);
	}
}

int Frequencies[3][16] =
	{
		{131, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 523},
		{33, 37, 41, 44, 49, 55, 62, 65, 73, 82, 87, 98, 110, 123, 131},
		{523, 587, 659, 698, 784, 880, 988, 1047, 1175, 1319, 1397, 1568, 1760, 1975, 2093}};

/*double Frequencies[16] = {1, 2, 3, 4,
						  5, 6, 7, 8,
						  9, 10, 11,
						  12, 13, 14, 15};*/
double peroid = 0.0;
int setFreq = -2;
void numberBoxCallback(struct BeforeCharWriteEventType *event)
{
	if (setFreq < 0)
		return;
	if (!event->line && event->position <= 13) // only allow 5 characters in the first line for frequency display
		return;
	else
		Set_CursorPosition(0, 9);
	if (event->c >= '0' || event->c <= '9')
		return; // only allow numbers in the frequency display box
	event->cancel = 1;
};

void keyPressCallback(enum KEYPAD key)
{
	if (modeCycle)
		return;
	AddFrequency(Frequencies[presetIndex][key]);
}

void keyReleaseCallback(enum KEYPAD key)
{
	if (modeCycle)
		return;
	RemoveFrequency(Frequencies[presetIndex][key]);
}

void switchPressCallback(enum SWITCHS key)
{

	switch (key)
	{

	case BUTTON_SWITCH2:
	{

		presetIndex = (presetIndex + 1) % 3;
	}
	break;

	case BUTTON_SWITCH3:
	{

		break;
	}
	case BUTTON_SWITCH4:
	{
		break;
	}
	case BUTTON_SWITCH5: // this will be our Mode Cycle
	{
		modeCycle = (modeCycle + 1) % 3;

		// if(modeCycle == 0)
		// {
		// 	Set_LCD( "mode 0");
		// 	presetIndex = (++presetIndex)%3;
		// }

		// else if(modeCycle == 1)
		// {
		// 	Set_LCD("mode 1");
		// }

		// else if(modeCycle ==2)
		// {
		// 		Set_LCD( "mode2");
		// 		presetIndex = presetIndex+2;
		// }

		break;
	}
	}
	update_SW_Menu();
}

// double ticksArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * @brief  The application entry point.
 * @retval int
 */
int run(void)
{

	EnableClock();
	enable_tim_clocks();
	// Init_LED(1);
	// Init_LED(0);
	LCD_Init();
	InitEvents();
	Init_buzzerEXT(0);
	Init_buzzerEXT(1);
	Init_buzzerEXT(2);
	Init_buzzerEXT(3);
	//   Init_buzzerEXT(0);
	//  Init_buzzerEXT(1);
	//  Init_buzzerEXT(0);
	//  Test_LED_With_Timer();
	/*DWT_Init();*/
	/*Write_Char_LCD('o');*/
	Write_String_LCD("EIEIEo");
	// Write_String_LCD(line2);
	Events.onKeyPadPress(keyPressCallback);
	Events.onKeyPadRelease(keyReleaseCallback);
	Events.onSwitchPress(switchPressCallback);
	Events.beforeCharWrite(numberBoxCallback);
	// Write_String_LCD("0123456789ABCDEF");
	// Write_String_LCD("0123456789ABCDEFG");
	// Clear_Display();
	update_SW_Menu();
	int lastTime = date();
	int index = 0;
	HAL_Delay(1000);
	//SetFrequency(1, 3);
	while (1)
	{
		CheckFrequency();
		check();
		checkLCDWrites();
		// double current = date();
		// double tickTime = date() - lastTickDate;

		// double total = 0;
		// for (int i = 0; i < 10; i++)
		// {
		// 	total += ticksArray[i];
		// 	if (i >= 9)
		// 		continue;
		// 	ticksArray[i + 1] = ticksArray[i];
		// }
		// ticksArray[0] = tickTime;
		// average = total / 10;

		if (date() - lastTime > 1000)
		{
			if (index == 0)
				SetFrequency(330, 3);
			else if (index == 1)
			{
				SetFrequency(0, 3);
			}
			else if (index == 4)
			{
				SetFrequency(250, 3);
			}
			else if (index == 5)
			{
				SetFrequency(0, 3);
			}
			index = (index + 1) % 8;
			lastTime = date();
		}
	}
}
