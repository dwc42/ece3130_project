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
void Test_LED_With_Timer(void)
{
	// Enable GPIOA clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	// Enable TIM1 clock
	__HAL_RCC_TIM1_CLK_ENABLE();

	// Configure PA0 as TIM1 Channel 1
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM1; // Alternate function for TIM1
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Configure TIM1 for PWM
	TIM_HandleTypeDef htim1 = {0};
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 7200 - 1; // 10 kHz timer clock
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 10000 - 1; // 1 Hz PWM frequency
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		// Initialization error
		while (1)
			;
	}

	TIM_OC_InitTypeDef sConfigOC = {0};
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 5000; // 50% duty cycle
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		// Configuration error
		while (1)
			;
	}

	// Enable main output for TIM1
	__HAL_TIM_MOE_ENABLE(&htim1);

	// Start PWM
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
	{
		// Start error
		while (1)
			;
	}
}
void Init_buzzer()
{
	uint32_t temp;

	temp = GPIOC->MODER;
	temp &= ~(0x03 << (2 * 9));
	temp |= (0x01 << (2 * 9));
	GPIOC->MODER = temp;

	temp = GPIOC->OTYPER;
	temp &= ~(0x01 << 9);
	GPIOC->OTYPER = temp;

	temp = GPIOC->PUPDR;
	temp &= ~(0x03 << (2 * 9));
	GPIOC->PUPDR = temp;

	// GPIOC->MODER &= ~(0x03 << (2 * 9));
	// GPIOC->OTYPER &= ~(0x01 << 9);
	// GPIOC->PUPDR &= ~(0x03 << (2 * 9));
}
void Init_LED()
{
	uint32_t temp;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; /* enable GPIOC clock */

	temp = GPIOA->MODER;
	temp &= ~(0x03 << (2 * 1));
	temp |= (0x01 << (2 * 1));
	GPIOC->MODER = temp;

	temp = GPIOA->OTYPER;
	temp &= ~(0x01 << 1);
	GPIOC->OTYPER = temp;

	temp = GPIOA->PUPDR;
	temp &= ~(0x03 << (2 * 1));
	GPIOC->PUPDR = temp;

	// GPIOC->MODER &= ~(0x03 << (2 * 9));
	// GPIOC->MODER |= (0x01 << (2 * 9));
	// GPIOC->OTYPER &= ~(0x01 << 9);
	// GPIOC->PUPDR &= ~(0x03 << (2 * 9));
}

uint8_t presetIndex = 0;

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
	if (setFreq < -1)
	{
		AddFrequency(Frequencies[presetIndex][key]);
		return;
	}
	if (setFreq == -1)
	{
		// Set the frequency to the current key pressed, this will be used for buzzer
		setFreq = key;
		char string[3] = {0, 0, 0};
		if (key > 99)
			return;
		sprintf(string, "%d", key);
		char writeString[11];
		strcpy(writeString, "Freq>K"); // Copy the base string into the buffer
		strcat(writeString, string);   // Append the `string`
		strcat(writeString, ": ");
		Set_LCD(writeString);

		// subscribe to the number box callback to limit input to numbers only in the frequency box
		return;
	}
	Write_Char_LCD(KEYPAD_CHARS[key]);
}
void keyReleaseCallback(enum KEYPAD key)
{
	if (setFreq < -1)
		RemoveFrequency(Frequencies[presetIndex][key]);
	// GPIOA->ODR &= ~(1 << 1);
	// Write_Char_LCD(KEYPAD_CHARS[key]);
}
// double average = 0;

void switchPressCallback(enum SWITCHS key)
{
	switch (key)
	{
	case BUTTON_SWITCH2:
	{
		presetIndex = (++presetIndex) % 3;
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
	case BUTTON_SWITCH5:
	{
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
	enable_tim_clocks();
	// Init_LED(1);
	// Init_LED(0);
	LCD_Init();
	InitEvents();
	Init_buzzerEXT(1);
	// Init_buzzerEXT(0);
	Init_buzzer();
	//   Init_buzzerEXT(0);
	//  Init_buzzerEXT(1);
	//  Init_buzzerEXT(0);
	//  Test_LED_With_Timer();
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
	// Write_String_LCD("0123456789ABCDEF");
	// Write_String_LCD("0123456789ABCDEFG");
	// Clear_Display();
	SetFrequency(500, 1);
	while (1)
	{
		// CheckFrequency();
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
