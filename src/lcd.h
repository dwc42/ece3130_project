#ifndef __LCD_H
#define __LCD_H
void Write_SR_LCD(uint8_t temp);
void LCD_nibble_write(uint8_t temp, uint8_t s);
void Write_Instr_LCD(uint8_t code);
void Write_Char_LCD(uint8_t code);

void Write_String_LCD(char *temp);
void Set_LCD(char *string);
void Clear_Display();
void Set_Cursor(uint8_t value);
void Set_Cursor_Blinking();
void Set_Display(uint8_t value);
void Set_Cursor_Write_Shift(uint8_t value);
void Set_Text_Write_Shift(uint8_t value);
void LCD_Init();
#endif /* __LCD_H */