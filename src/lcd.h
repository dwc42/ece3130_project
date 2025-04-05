#ifndef __LCD_H
#define __LCD_H
struct LCDCache
{
	char string[33]; // 32 characters + null terminator, 16x2 LCD can hold 32 chars in total (16 chars per line * 2 lines)
	uint8_t line;
	uint8_t position;
};
void Write_SR_LCD(uint8_t temp);
void checkLCDWrites();
void LCD_nibble_write(uint8_t temp, uint8_t s);
void Write_Instr_LCD(uint8_t code);
void Write_Char_LCD(uint8_t code);
void Set_CursorPosition(uint8_t line, uint8_t position);
void Write_String_LCD(char *temp);
void Set_LCD(char *string);
void Clear_Display();
void Set_Cursor(uint8_t value);
void Set_Cursor_Blinking();
/**
 * @brief Get a substring from the LCD cache string, from index 'from' to 'to'
 * @param from starting index (inclusive) values are clamped to 0-31, negative values are counted from the end of the string (e.g. -1 is the last character)
 * @param to ending index (inclusive) values are clamped to 0-31, negative values are counted from the end of the string (e.g. -1 is the last character)
 * @return char* the substring allocated in heap memory, caller must free it
 * @note if 'from' is greater than 'to', the substring will be returned in reverse order. For example, Get_String_LCD(5, 2) will return the substring from index 2 to 5 in reverse order.
 */
char *Get_String_LCD(int8_t from, int8_t to);
void Set_Display(uint8_t value);
void Set_Cursor_Write_Shift(uint8_t value);
void Set_Text_Write_Shift(uint8_t value);
void LCD_Init();
#endif /* __LCD_H */