#include <stdint.h>
#include <stdlib.h>
#include "events.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "keypad.h"
#include "lcd.h"
void SystemClock_Config(void);

void EnableClock()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
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

void keyPressCallback(enum KEYPAD key)
{
	if (!key)
		return;
	Write_Char_LCD(KEYPAD_CHARS[key]);
}
void switchPressCallback(enum SWITCHS key)
{
	if (!key)
		return;
	switch (key)
	{
	case BUTTON_SWITCH2:
	{
		Write_String_LCD(" SW2 ");
		break;
	}
	case BUTTON_SWITCH3:
	{
		Write_String_LCD(" SW3 ");
		break;
	}
	case BUTTON_SWITCH4:
	{
		Write_String_LCD(" SW4 ");
		break;
	}
	case BUTTON_SWITCH5:
	{
		Write_String_LCD(" SW5 ");
		break;
	}
	}
}
/**
 * @brief  The application entry point.
 * @retval int
 */
int run(void)
{
	int LED = 1;
	EnableClock();
	Delay(500);
	Init_LED(LED);
	LCD_Init();
	//InitEvents();
	//Clear_Display();
	//Set_Cursor(1);
	//Set_Display(1);
	//Set_Cursor_Blinking();
	//Set_Cursor_Write_Shift(1);
	//Set_Text_Write_Shift(1);
	Delay(2000);
	char *line1 = "Welcome";
	//char *line2 = "Spring 2023";
	/*Write_Char_LCD('o');*/
	Write_Char_LCD(0x42);
	Write_String_LCD(line1); 
	//Write_Instr_LCD(0xc0); /* move to line 2*/ 
	//Write_String_LCD(line2);

	//Events.onKeyPadPress(keyPressCallback);
	//Events.onSwitchPress(switchPressCallback);
	while (1)
	{
		//Write_Char_LCD('1');
		//Delay(1000);
		//check();
		GPIOA->ODR ^=(1<<1);
		Delay(1000);
	}

	
}
