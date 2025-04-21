#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "events.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "recording.h"
#include "lcd.h"
#include "buzzer.h"
#include "date.h"
#include <string.h>
#include <math.h>
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
int Frequencies[3][16] =
	{
		{175, 262, 392, 0, 165, 247, 349, 523, 147, 220, 330, 494, 131, 196, 292, 440},
		{44, 65, 98, 0, 41, 62, 87, 131, 37, 55, 82, 123, 33, 49, 73, 110},
		{698, 1047, 1568, 0, 659, 988, 1397, 2093, 587, 880, 1319, 1975, 523, 784, 1175, 1760}};
struct NoteProperties
 	{
		uint16_t frequencies;
 		char octaves;
 		char note;

 	};
 struct NoteProperties newFrequencies[3][16] =
 	{
		{ {175,'3','F',},{262,'4','C'},{392,'4','G'},{165,'3','E'},{247,'3','B'},{349,'4','F'},{523,'5','C'},{147,'3','D'},{220,'3','A'},{330,'4','E'},{494,'4','B'},{131,'3','C'},{196,'3','G'},{292,'4','D'},{440,'4','A'} },
		{ {44,'1','F'},{65,'2','C'},{98,'2','G'},{41,'1','E'},{62,'1','B'},{87,'2','F'},{131,'3','C'},{37,'1','D'},{55,'1','A'},{82,'2','E'},{123,'2','B'},{33,'1','C'},{49,'1','G'},{73,'2','D'},{110,'2','A'} },
		{ {698,'5','F'},{1047,'6','C'},{1568,'6','G'},{659,'5','E'},{988,'5','B'},{1397,'6','F'},{2093,'7','C'},{587,'5','D'},{880,'5','A'},{1319,'6','E'},{1975,'6','B'},{523,'5','C'},{784,'5','G'},{1175,'6','D'},{1760,'6','A'} },
	};


	 void DisplayNumber(long num, int8_t line, int8_t position, uint8_t from)
	 {

			int logOf = (int)log10(num);
			if((line != -1) && (position != -1))
			Set_CursorPosition(line, from ? position - logOf - ((num < 0) ? 1 : 0) : position);
			if(num < 0)
				Write_Char_LCD('-');

			for (int i = logOf; i >= 0; i--)
			{
				uint8_t place = (int)num / (int)pow(10, i);
				uint8_t digit = place % 10;
				Write_Char_LCD(digit + '0');
			}
 		}
 	void displayFrequency(enum KEYPAD key)
 	{
 	  struct NoteProperties noteProperties = newFrequencies[presetIndex][key];

		Set_CursorPosition(0,0);
		Write_Char_LCD(noteProperties.note);
		Write_Char_LCD(noteProperties.octaves);
		Write_Char_LCD(' ');
		DisplayNumber(noteProperties.frequencies,-1, -1, 0);
		Write_String_LCD("Hz");
 	}








char *intToString(int number)
{
	// Handle special case for 0
	if (number == 0)
	{
		char *str = malloc(2); // "0" + null terminator
		if (str == NULL)
		{
			printf("Memory allocation failed\n");
			return NULL;
		}
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	// Calculate the number of digits using log10
	int numDigits = (int)log10(abs(number)) + 1; // Number of digits in the number
	if (number < 0)
	{
		numDigits++; // Add 1 for the negative sign
	}

	// Allocate memory for the string (digits + null terminator)
	char *str = malloc(numDigits + 1);
	if (str == NULL)
	{
		printf("Memory allocation failed\n");
		return NULL;
	}

	// Convert the integer to a string
	sprintf(str, "%d", number);
	str[numDigits - 1] = '\0'; // Null-terminate the string
	return str;
}


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
	char sector4New[4] = {0, 0, 0, 0};
	char sector5New[4] = {0, 0, 0, 0};
	char sector6New[4] = {0, 0, 0, 0};
	char sector7New[4] = {0, 0, 0, 0};
	;
	switch (modeCycle)
	{
	case 0:
	{
		if (presetIndex == 0)
		{
			strcpy(sector4New, "3&4");
		}
		else if (presetIndex == 1)
		{

			strcpy(sector4New, "1&2");
		}
		else if (presetIndex == 2)
		{
			strcpy(sector4New, "5&6");
		}
		if (recording)
			strcpy(sector5New, "REC");
		else
		{
			strcpy(sector5New, "SRC");
		}
		if (playBack)
			strcpy(sector6New, "PBE");
		else
		{
			strcpy(sector6New, "PBD");
		}
		// strcpy(sector6New, "TPB");
		break;
	}
	case 1:
	{
		strcpy(sector4New, "PDD");
		strcpy(sector5New, "TRP");
		strcpy(sector7New, "M#1");
		// strcpy(sector6New, "   ");
		break;
	}
	case 2:
	{
		strcpy(sector4New, "   ");
		strcpy(sector5New, "TRP");
		strcpy(sector7New, "M#2");
		// strcpy(sector6New, "   ");
		break;
	}
	}

	if (sector4New[0] && !compareStrings(sector4New, switch_Menu[0]))
	{
		strcpy(switch_Menu[0], sector4New);
		Write_String_Sector_LCD(4, sector4New);
	}

	if (sector7New[0] && !compareStrings(sector7New, switch_Menu[3]))
	{
		strcpy(switch_Menu[3], sector7New);
		Write_String_Sector_LCD(7, sector7New);
	}
	if (sector5New[0] && !compareStrings(sector5New, switch_Menu[1]))
	{
		strcpy(switch_Menu[1], sector5New);
		Write_String_Sector_LCD(5, sector5New);
	}
	if (sector6New[0] && !compareStrings(sector6New, switch_Menu[2]))
	{
		strcpy(switch_Menu[2], sector6New);
		Write_String_Sector_LCD(6, sector6New);
	}
}



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
	AddFrequency(Frequencies[presetIndex][key], 0);
	recordMusicPress(key);
	displayFrequency(key);
}

void keyReleaseCallback(enum KEYPAD key)
{
	if (modeCycle)
		return;
	RemoveFrequency(Frequencies[presetIndex][key], 0);
	recordMusicRelease(key);
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
		toggleRecording();
		break;
	}
	case BUTTON_SWITCH4:
	{
		togglePlayBack();
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
long test = 0;
int digit = 0;
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
	initOCT();
	Init_buzzerEXT(0);
	Init_buzzerEXT(1);
	Init_buzzerEXT(2);
	Init_buzzerEXT(3);
	// HAL_Delay(1000);

	//    Init_buzzerEXT(0);
	//   Init_buzzerEXT(1);
	//   Init_buzzerEXT(0);
	//   Test_LED_With_Timer();
	/*DWT_Init();*/
	/*Write_Char_LCD('o');*/
	Write_String_LCD("Sounds");
	// Write_String_LCD(line2);
	Events.onKeyPadPress(keyPressCallback);
	Events.onKeyPadRelease(keyReleaseCallback);
	Events.onSwitchPress(switchPressCallback);
	Events.beforeCharWrite(numberBoxCallback);
	// Write_String_LCD("0123456789ABCDEF");
	// Write_String_LCD("0123456789ABCDEFG");
	// Clear_Display();
	update_SW_Menu();

	int index = 0;
	HAL_Delay(1000);
	int lastTime = (int)date();
	int startTime = (int)date();
	// SetFrequency(1, 3);
	int index1 = 0;
	//AddFrequency(1000, 1000 + lastTime);

	while (1)
	{
		CheckFrequency();
		check();
		checkLCDWrites();
		playRecording();
		checkDate();
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
		if ((int)date() - startTime < 3000)
			continue;
		if (date() - lastTime > 1000)
		{

			test = date(); //(int)date();
			int logOf = (int)log10(test);
			Set_CursorPosition(0, 15 - logOf);

			for (int i = logOf; i >= 0; i--)
			{
				int place = (int)test / (int)pow(10, i);
				digit = place % 10;
				Write_Char_LCD(digit + '0');
			}
			lastTime = date();
		}
	}
}
