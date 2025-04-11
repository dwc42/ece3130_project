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

int current_Mode = 0;
uint8_t presetIndex = 0;

int Frequencies[3][16] = 
{
{131, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 523},
{33, 37, 41, 44, 49, 55, 62, 65, 73, 82, 87, 98, 110, 123, 131},
{523, 587, 659, 698, 784, 880, 988, 1047, 1175, 1319, 1397, 1568, 1760, 1975, 2093}
};

/*double Frequencies[16] = {1, 2, 3, 4,
						  5, 6, 7, 8,
						  9, 10, 11,
						  12, 13, 14, 15};*/
double peroid = 0.0;
int setFreq = -2;
int modeCycle;
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
	if (modeCycle == 0)
	{
		AddFrequency(Frequencies[presetIndex][key]);
	}
	else if(modeCycle == 1)
	{
		AddFrequency(Frequencies[presetIndex+1][key]);
	}
	
	else if(modeCycle ==2)
	{
		AddFrequency(Frequencies[presetIndex+2][key]);
	}
	return;
}


void keyReleaseCallback(enum KEYPAD key)
{
	if (modeCycle ==0) 
	{
		RemoveFrequency(Frequencies[presetIndex][key]);
	}
	else if(modeCycle ==1)
	{
		RemoveFrequency(Frequencies[presetIndex+1][key]);
	}
	else if(modeCycle ==2)
	{
		RemoveFrequency(Frequencies[presetIndex+2][key]);
	}
	
	
	return;
}

void switchPressCallback(enum SWITCHS key)
{
	
	switch (key)
	{
		
		
	case BUTTON_SWITCH2:
	{
		
		modeCycle = (++modeCycle)%3;
				if( current_Mode ==0)
				{	
			
					if(modeCycle ==0)
					{
						Set_LCD ("3&4");
					
					}
					else if(modeCycle==1)
				{
				
					Set_LCD("1&2");
				}
				else if(modeCycle==2)
				{
					Set_LCD("5&6");
				}
		
			}
		break;
	}
	case BUTTON_SWITCH3:
	{
		break;
	}
	case BUTTON_SWITCH4:
	{
		break;
	}
	case BUTTON_SWITCH5:       // this will be our Mode Cycle 
	{
			  current_Mode =(++current_Mode)%3;
	
		
		if(current_Mode == 0)
		{
			Set_LCD( "mode 0");
			presetIndex = (++presetIndex)%3;
		}
		
		else if(current_Mode == 1)
		{
			Set_LCD("mode 1");
		}
		
		else if(current_Mode ==2)
		{
				Set_LCD( "mode2");
				presetIndex = presetIndex+2;
		}
		
		break;
	}
}
}

// double ticksArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * @brief  The application entry point.
 * @retval int
 */
int run(void)
{

	EnableClock();
	// Init_LED(1);
	// Init_LED(0);
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
	Events.beforeCharWrite(numberBoxCallback);
	Init_buzzer();
	// Write_String_LCD("0123456789ABCDEF");
	// Write_String_LCD("0123456789ABCDEFG");
	// Clear_Display();
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
		// /*if (date() - lastPrint > 1000)
		// {
		// 	char *str = doubleToString(average, 3);
		// 	Set_LCD(str);
		// 	lastPrint = date();
		// }*/
		// lastTickDate = date();
	}
}


