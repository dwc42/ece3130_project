#ifndef __LCD_H
#define __LCD_H
#include "stm32l4xx_hal.h"
#include "events.h"
void Write_SR_LCD(uint8_t temp);
void LCD_nibble_write(uint8_t temp, uint8_t s);
void Write_Instr_LCD(uint8_t code);
void Write_Char_LCD(uint8_t code);

void Write_String_LCD(char *temp);
void LCD_Init();
#endif /* __LCD_H */