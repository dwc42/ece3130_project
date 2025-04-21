#include "stm32l4xx_hal.h"
#include "events.h"
#include "date.h"
#include "lcd.h"
#include "list.h"
#include "stdio.h"
#include "math.h"
#define min(a, b) ((a) < (b) ? (a) : (b))
// void Write_SR_LCD_Direct(uint8_t temp)
// {
// 	uint8_t i;
// 	uint8_t mask = 0b10000000;
// 	for (i = 0; i < 8; i++)
// 	{
// 		if ((temp & mask) == 0)
// 			GPIOB->ODR &= ~(1 << 5);
// 		else
// 			GPIOB->ODR |= (1 << 5);

// 		/*	Sclck */
// 		GPIOA->ODR &= ~(1 << 5);
// 		GPIOA->ODR |= (1 << 5);
// 		Delay(1);
// 		mask = mask >> 1;
// 	}

// 	/*Latch*/
// 	GPIOA->ODR |= (1 << 10);
// 	GPIOA->ODR &= ~(1 << 10);
// }
uint8_t maskLCD = 0b10000000;
uint8_t delayLCD = 0;
void Shift_Cursor_LCD(uint8_t value)
{

	uint8_t offset = (value) ? 1 : -1;
	cacheLCD.position += offset; // reset the position to 0 after shifting
	if (cacheLCD.position > 250)
	{
		Set_CursorPosition(!cacheLCD.line, 15);
	}
	else if (cacheLCD.position > 15)
	{
		Set_CursorPosition(!cacheLCD.line, 0);
	}
	else
	{
		Write_Instr_LCD(0x10 | (value << 2));
	}
}
uint8_t Write_SR_LCD_Direct_GEN(uint8_t temp)
{
	if (maskLCD == 0b00000000)
	{
		if (delayLCD++ < 1)
			return 0;
		GPIOA->ODR |= (1 << 10);
		GPIOA->ODR &= ~(1 << 10);
		maskLCD = 0b10000000; // Reset the mask for the next write
		delayLCD = 0;
		return 1;
	}

	if ((temp & maskLCD) == 0)
		GPIOB->ODR &= ~(1 << 5);
	else
		GPIOB->ODR |= (1 << 5);

	GPIOA->ODR &= ~(1 << 5);
	GPIOA->ODR |= (1 << 5);

	maskLCD = maskLCD >> 1;
	return 0;
}
uint8_t *sr_writes = NULL;
int sr_writeslength = 0;
void Write_SR_LCD(uint8_t temp)
{
	if (sr_writes == NULL)
	{
		sr_writes = malloc(sizeof(uint8_t));
		sr_writes[0] = temp;
		sr_writeslength = 1;
		return;
	}
	uint8_t *new_list = malloc(sizeof(uint8_t) * (sr_writeslength + 1));
	new_list[0] = temp;
	for (int i = 0; i < sr_writeslength; i++)
	{
		new_list[i + 1] = sr_writes[i];
	}

	free(sr_writes);
	sr_writes = new_list;
	sr_writeslength++;
}
uint8_t currentWrite = 0;
uint8_t writingInstruction = 0;
uint8_t popWrites()
{
	if (sr_writes == NULL)
	{
		writingInstruction = 0;
		return 0;
	}
	if (sr_writeslength == 0)
	{
		writingInstruction = 0;
		return 0;
	}
	if (sr_writeslength == 1)
	{
		uint8_t last = sr_writes[0]; // if there is only one element, return it and set the list to NULL
		free(sr_writes);
		sr_writes = NULL;
		writingInstruction = 1;
		sr_writeslength = 0;
		return last;
	}
	uint8_t last = sr_writes[sr_writeslength - 1];
	uint8_t *new_list = malloc(sizeof(uint8_t) * (sr_writeslength - 1));
	for (int i = 0; i < sr_writeslength - 1; i++)
	{
		new_list[i] = sr_writes[i];
	}
	free(sr_writes);
	sr_writes = new_list;
	sr_writeslength--;
	writingInstruction = 1;
	return last;
}
double lastDateLCD = 0;

void checkLCDWrites()
{
	double currentDate = date();
	if (currentDate - lastDateLCD < 1) // 1ms delay between writes to LCD
		return;						   // not enough time has passed since last write to LCD, return and wait for next check
	lastDateLCD = currentDate;		   // update the last date LCD was written to
	if (writingInstruction == 0)
	{
		currentWrite = popWrites();
		if (writingInstruction == 0)
			return;
	}
	uint8_t done = Write_SR_LCD_Direct_GEN(currentWrite);
	if (!done)
		return;
	writingInstruction = 0;
}
struct LCDCache cacheLCD;

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
// void LCD_nibble_write_direct(uint8_t temp, uint8_t s)
// {

// 	/*writing instruction*/
// 	if (s == 0)
// 	{
// 		temp = temp & 0xF0;
// 		temp = temp | 0x02; /*RS (bit 0) = 0 for Command EN (bit1)=high */
// 		Write_SR_LCD_Direct(temp);

// 		temp = temp & 0xFD; /*RS (bit 0) = 0 for Command EN (bit1) = low*/
// 		Write_SR_LCD_Direct(temp);
// 	}

// 	/*writing data*/
// 	else if (s == 1)
// 	{
// 		temp = temp & 0xF0;
// 		temp = temp | 0x03; /*RS(bit 0)=1 for data EN (bit1) = high*/
// 		Write_SR_LCD_Direct(temp);

// 		temp = temp & 0xFD; /*RS(bit 0)=1 for data EN(bit1) = low*/
// 		Write_SR_LCD_Direct(temp);
// 	}
// }
// void Write_Instr_LCD_Direct(uint8_t code)
// {
// 	LCD_nibble_write_direct(code & 0xF0, 0);

// 	code = code << 4;
// 	LCD_nibble_write_direct(code, 0);
// }

void Write_Instr_LCD(uint8_t code)
{
	LCD_nibble_write(code & 0xF0, 0);

	code = code << 4;
	LCD_nibble_write(code, 0);
}

void Write_Char_LCD(uint8_t code)
{
	if (Events.beforeCharWriteSubscribed)
	{
		struct BeforeCharWriteEventType event;
		event.c = code;						 // character to be written
		event.cancel = 0;					 // default to not canceling the write
		event.position = cacheLCD.position;	 // current position in the LCD cache
		event.line = cacheLCD.line;			 // current line in the LCD cache
		runBeforeCharWriteCallbacks(&event); // run the before character write callbacks
		if (event.c != code)				 // if the character was modified by a callback, use the modified character instead
			code = event.c;
		if (event.cancel)
			return;
	}
	if (cacheLCD.position >= 16)
	{
		Set_CursorPosition(cacheLCD.line ? 0 : 1, 0);
	}
	cacheLCD.string[16 * cacheLCD.line + cacheLCD.position] = code; // cache the string for later use in Set_LCD
	LCD_nibble_write(code & 0xF0, 1);
	code = code << 4;
	LCD_nibble_write(code, 1);
	cacheLCD.position++;
}

void Write_String_LCD(char *temp)
{
	int i = 0;
	while (temp[i] != '\0')
	{
		Write_Char_LCD(temp[i]);
		i = i + 1;
	}
}
void Clear_Display()
{
	Write_Instr_LCD(0x01);
	for (uint8_t i = 0; i < 32; i++)
	{
		cacheLCD.string[i] = 0;
	}
	cacheLCD.line = 0;
	cacheLCD.position = 0;
}

char *Get_String_LCD(int8_t from, int8_t to)
{
	if (from > 31)
		from = 31;
	if (to > 31)
		to = 31;
	if (from < 0)
		from = min(32 + from, 0);
	if (to < 0)
		to = min(32 + to, 0);
	char *output = malloc(sizeof(char) * ((abs(to - from) + 1)));
	int8_t j = 0;
	if (from > to)
		for (int8_t i = to; i >= from; i--)
			output[j++] = cacheLCD.string[i];
	else
		for (int8_t i = from; i <= to; i++)
			output[j++] = cacheLCD.string[i];
	output[j] = '\0';
	return output;
}
void Set_LCD(char *string)
{
	Clear_Display();
	// HAL_Delay(5);
	Write_String_LCD(string);
}
void Set_CursorPosition(uint8_t line, uint8_t position)
{

	uint8_t location = line ? 0xC0 : 0x80;
	location |= position;
	cacheLCD.line = line;
	cacheLCD.position = position;
	Write_Instr_LCD(location);
}

void LCD_Init()
{
	for (int i = 0; i < 32; i++)
		cacheLCD.string[i] = ' ';
	cacheLCD.string[32] = '\0'; // ensure null termination for safety, though it should not be used in the string directly
	cacheLCD.line = 0;			// default line 0
	cacheLCD.position = 0;		// default position 0
	GPIOA->MODER &= ~(0x03 << (2 * 5));
	GPIOA->MODER |= (0x01 << (2 * 5));
	GPIOA->MODER &= ~(0x03 << (2 * 10));
	GPIOA->MODER |= (0x01 << (2 * 10));
	GPIOA->OTYPER &= ~(0x01 << 5);
	GPIOA->OTYPER &= ~(0x01 << 10);
	GPIOA->PUPDR &= ~(0x03 << (2 * 5));
	GPIOA->PUPDR &= ~(0x03 << (2 * 10));
	GPIOB->MODER &= ~(0x03 << (2 * 5));
	GPIOB->MODER |= (0x01 << (2 * 5));
	;
	GPIOB->OTYPER &= ~(0x01 << 5);
	GPIOB->PUPDR &= ~(0x03 << (2 * 5));
	/* LCD controller reset sequence */
	LCD_nibble_write(0x30, 0);
	LCD_nibble_write(0x30, 0);
	LCD_nibble_write(0x30, 0);
	LCD_nibble_write(0x20, 0);

	Write_Instr_LCD(0x28);
	/* set 4 bit data LCD - two line display - 5x8 font*/
	Write_Instr_LCD(0x0E);
	/* turn on display, turn on cursor , turn off blinking */
	Write_Instr_LCD(0x01);
	/* clear display screen and return to home position*/
	Write_Instr_LCD(0x06);
	/* move cursor to right (entry mode set instruction)*/
}

void Write_String_Sector_LCD(uint8_t sector, char *string)
{
	uint8_t lastPosition = cacheLCD.position;
	uint8_t lastLine = cacheLCD.line;
	Set_CursorPosition(sector / 4, (sector % 4) * 4);
	Write_String_LCD(string);
	Set_CursorPosition(lastLine, lastPosition);
}