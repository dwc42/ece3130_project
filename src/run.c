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
#include "run.h"
#include <string.h>
#include <math.h>
void SystemClock_Config(void);

void EnableClock()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
}
                                                          // matrix of frequencies and the corresponding note and octave

struct NoteProperties newFrequencies[3][16] =
	{
		{{175, '3', 'F'}, {262, '4', 'C'}, {392, '4', 'G'}, {0, 0, 0}, {165, '3', 'E'}, {247, '3', 'B'}, {349, '4', 'F'}, {523, '5', 'C'}, {147, '3', 'D'}, {220, '3', 'A'}, {330, '4', 'E'}, {494, '4', 'B'}, {131, '3', 'C'}, {196, '3', 'G'}, {292, '4', 'D'}, {440, '4', 'A'}},
		{{44, '1', 'F'}, {65, '2', 'C'}, {98, '2', 'G'}, {0, 0, 0}, {41, '1', 'E'}, {62, '1', 'B'}, {87, '2', 'F'}, {131, '3', 'C'}, {37, '1', 'D'}, {55, '1', 'A'}, {82, '2', 'E'}, {123, '2', 'B'}, {33, '1', 'C'}, {49, '1', 'G'}, {73, '2', 'D'}, {110, '2', 'A'}},
		{{698, '5', 'F'}, {1047, '6', 'C'}, {1568, '6', 'G'}, {0, 0, 0}, {659, '5', 'E'}, {988, '5', 'B'}, {1397, '6', 'F'}, {2093, '7', 'C'}, {587, '5', 'D'}, {880, '5', 'A'}, {1319, '6', 'E'}, {1975, '6', 'B'}, {523, '5', 'C'}, {784, '5', 'G'}, {1175, '6', 'D'}, {1760, '6', 'A'}},
};

void DisplayNumber(long num, int8_t line, int8_t position, uint8_t from, uint8_t minDigits)
{

	int logOf = (!num) ? 1 : (int)log10(num);
	if (logOf < minDigits) logOf = minDigits -1;
	if ((line != -1) && (position != -1))
		Set_CursorPosition(line, from ? position - logOf - ((num < 0) ? 1 : 0) : position);
	if (num < 0)
		Write_Char_LCD('-');

	for (int i = logOf; i >= 0; i--)
	{
		uint8_t place = (int)num / (int)pow(10, i);
		uint8_t digit = place % 10;
		Write_Char_LCD(digit + '0');
	}
}
void displayFrequency(enum KEYPAD key)                             // displays frequency and octave and note
{
	struct NoteProperties noteProperties = newFrequencies[presetIndex][key];

	Set_CursorPosition(0, 0);
	Write_Char_LCD(noteProperties.note);
	Write_Char_LCD(noteProperties.octaves);
	Write_Char_LCD(' ');
	DisplayNumber(noteProperties.frequency, -1, -1, 0, 4);
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
void update_SW_Menu()                          // initializes each sector
{
	char sector4New[4] = {0, 0, 0, 0};
	char sector5New[4] = {0, 0, 0, 0};
	char sector6New[4] = {0, 0, 0, 0};
	char sector7New[4] = {0, 0, 0, 0};
	;
<<<<<<< Updated upstream
	switch (modeCycle)          // switch case for each mode we have
=======
	switch (modeCycle)                     // based on mode selection, the LCD sectors will update 
>>>>>>> Stashed changes
	{
	case 0:
	{                                                          // each strcpy line below prints 3 char limit to each sector
		if (presetIndex == 0)
		{
			strcpy(sector4New, "3&4");    // prints octaves 3 and 4 to LCD
		}
		else if (presetIndex == 1)
		{

			strcpy(sector4New, "1&2");       // shows octaves 1 and 2
		}
		else if (presetIndex == 2)     // mode 2
		{
			strcpy(sector4New, "5&6");     // prints octaves 5 and 6 to LCD
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
		
		strcpy(sector7New, "M#0");
		break;
	}
	case 1:
	{
		strcpy(sector4New, "PDD");    // these following lines update LCD in affected sectors with new chars
		strcpy(sector5New, "TRP");
		strcpy(sector7New, "M#1");
		
		break;
	}
	case 2:
	{
		strcpy(sector4New, "   ");
		strcpy(sector5New, "TRP");
		strcpy(sector7New, "M#2");
		
		break;
	}
	}

<<<<<<< Updated upstream
	if (sector4New[0] && !compareStrings(sector4New, switch_Menu[0]))      // these following lines are what allow us to to change the
	{                                                                      // LCD based on inputs in switch statement above
=======
	if (sector4New[0] && !compareStrings(sector4New, switch_Menu[0]))    
	{
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
void keyPressCallback(enum KEYPAD key)
=======
void keyPressCallback(enum KEYPAD key)  
>>>>>>> Stashed changes
{
	if (modeCycle)
		return;
	AddFrequency(newFrequencies[presetIndex][key].frequency, 0);
	recordMusicPress(key);
	displayFrequency(key);
	DisplayNumber(key,0, 15, 1, 2);
}

void keyReleaseCallback(enum KEYPAD key)
{
	if (modeCycle)
		return;
	RemoveFrequency(newFrequencies[presetIndex][key].frequency, 0);
	recordMusicRelease(key);
}

void switchPressCallback(enum SWITCHS key)
{

	switch (key)
	{

	case BUTTON_SWITCH2:                             // preset index for SW2 is used to switch octaves
	{

		presetIndex = (presetIndex + 1) % 3;
	}
	break;

	case BUTTON_SWITCH3:
	{
		toggleRecording();                    // if SW3 is pressed, calls toggle recording mode
		break;
	}
	case BUTTON_SWITCH4:    
	{
		togglePlayBack();               // if SW4 is pressed, calls toggle playback mode 
		break;
	}
	case BUTTON_SWITCH5: // this will be our Mode Cycle
	{
		modeCycle = (modeCycle + 1) % 3;


		break;
	}
	}
	update_SW_Menu();   
}

#define HEAP_START 0x20000000 // Adjust based on your MCU's memory layout

uint32_t checkMemoryUsage()
{
	uint32_t msp;							   // Main Stack Pointer
	__asm volatile("MRS %0, msp" : "=r"(msp)); // Get the current value of MSP

	return msp;
}
// double ticksArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
long test = 0;
int digit = 0;
uint32_t freeMemory = 0;
/**
 * @brief  The application entry point.
 * @retval int
 */
int run(void)
{

	EnableClock();
	enable_tim_clocks();
	initOCT();
	// Init_LED(1);
	// Init_LED(0);
	LCD_Init();
	InitEvents();
	Init_buzzerEXT(0);
	Init_buzzerEXT(1);
	Init_buzzerEXT(2);
	Init_buzzerEXT(3);
	
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

	HAL_Delay(1000);

	while (1)
	{
		CheckFrequency();
		check();
		checkLCDWrites();
		playRecording();
		
		
	}
}
