#ifndef __LCD_H
#define __LCD_H
/** * @brief LCD cache structure, used to store the current state of the LCD display
 * @struct LCDCache
 * @note string: the string currently displayed on the LCD, 32 characters + null terminator
 *
 * @note line: the current line of the LCD (0 or 1) 0 top, 1 bottom
 * @note position: current position of the cursor in the string (0-15)
 */
struct LCDCache
{
	char string[33]; // 32 characters + null terminator, 16x2 LCD can hold 32 chars in total (16 chars per line * 2 lines)
	uint8_t line;
	uint8_t position;
};
void Write_SR_LCD(uint8_t temp);
/**
 * @brief function to manage lcd writes queue
 * @note This function will be called in the main loop to check if there are any writes to be done
 */
void checkLCDWrites();
/**
 * @brief Writes a instruction to the LCD display
 * @param temp the instruction that will be written to the LCD display
 * @param s 0 for instruction, 1 for data
 * @note This function will not set the cursor position to the end of the string
 */
void LCD_nibble_write(uint8_t temp, uint8_t s);
/**
 * @brief Writes a instruction to the LCD display
 * @param code the instruction that will be written to the LCD display
 * @note This instruction will also set the cursor position to the end of the string
 */
void Write_Instr_LCD(uint8_t code);
/**
 * @brief Writes a character to the LCD display
 * @param code the character that will be written to the LCD display
 * @note This function will also set the cursor position to the end of the string
 */
void Write_Char_LCD(uint8_t code);
/**
 * @brief Sets the cursor position on the LCD display
 * @param line the line of the LCD display (0 or 1)
 * @param position the position of the cursor in the line (0-15)
 * @note This function will also set the cacheLCD.line and cacheLCD.position variables
 */
void Set_CursorPosition(uint8_t line, uint8_t position);
/**
 * @brief Writes a string to the LCD display
 * @param code the string that will be written to the LCD display
 * @note This function will also set the cursor position to the end of the string
 */
void Write_String_LCD(char *temp);
/**
 * @brief Writes a string to the LCD display, overwriting the current string
 * @param string the string to be written to the LCD display
 * @note This function will also set the cursor position to the end of the string
 */
void Set_LCD(char *string);
/**
 * @brief Clears the LCD display and resets the cursor position to 0,0
 * @note This function will also clear the LCD cache string
 */
void Clear_Display();
/**
 * @brief sets wether the display the cursor is on or off
 * @param value 0 to turn off the cursor, 1 to turn on the cursor
 */
void Set_Cursor(uint8_t value);
/**
 * @brief Sets the cursor blinking on or off
 * @param value 0 to turn off the cursor blinking, 1 to turn on the cursor blinking
 */
void Set_Cursor_Blinking();
/**
 * @brief Shifts the LCD display left(0) or right(1)
 */
void shiftLCD(uint8_t value);
/** * @brief LCD cache structure, used to store the current state of the LCD display
 * @struct LCDCache
 * @note string: the string currently displayed on the LCD, 32 characters + null terminator
 *
 * @note line: the current line of the LCD (0 or 1) 0 top, 1 bottom
 * @note position: current position of the cursor in the string (0-15)
 */
extern struct LCDCache cacheLCD;
/**
 * @brief Get a substring from the LCD cache string, from index 'from' to 'to'
 * @param from starting index (inclusive) values are clamped to 0-31, negative values are counted from the end of the string (e.g. -1 is the last character)
 * @param to ending index (inclusive) values are clamped to 0-31, negative values are counted from the end of the string (e.g. -1 is the last character)
 * @return char* the substring allocated in heap memory, caller must free it
 * @note if 'from' is greater than 'to', the substring will be returned in reverse order. For example, Get_String_LCD(5, 2) will return the substring from index 2 to 5 in reverse order.
 */
char *Get_String_LCD(int8_t from, int8_t to);
/**
 * @brief Sets weither the display is on or off
 * @param value 0 to turn off the display, 1 to turn on the display
 */
void Set_Display(uint8_t value);
/**
 * @brief Sets weither the cursor Shifts left or right on character write
 * @param value 0 to shift left, 1 to shift right
 */
void Set_Cursor_Write_Shift(uint8_t value);

void Set_Text_Write_Shift(uint8_t value);
/**
 * @brief Inializes the LCD
 */
void LCD_Init();
#endif /* __LCD_H */