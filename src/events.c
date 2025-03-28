#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "events.h"
#include "stm32l4xx_hal.h"

struct EventsType Events;
void Delay(unsigned int n)
{
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 136; i++)
			;
	/*1ms*/
}

enum KEYPAD KEYPAD_MATRIX[4][4] = {{KEY_1, KEY_2, KEY_3, KEY_A},
								   {KEY_4, KEY_5, KEY_6, KEY_B},
								   {KEY_7, KEY_8, KEY_9, KEY_C},
								   {KEY_ASTRISK, KEY_0, KEY_HASH, KEY_D}};
enum SWITCHS SWITCH_ARRAY[4] = {
	BUTTON_SWITCH2,
	BUTTON_SWITCH3,
	BUTTON_SWITCH4,
	BUTTON_SWITCH5,
};
char KEYPAD_CHARS[16] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'*', '0', '#', 'D'};
const int keyPadIB[4] = {1, 2, 3, 4};
const int keyPadIB2[4] = {8, 9, 10, 11};
void subscribeKeyPad()
{

	if (Events.keypadsubcribed != 0)
		return;
	Events.keypadsubcribed = 1;
	for (int i = 0; i < 4; i++)
	{

		// input for rows
		int keyB2 = keyPadIB2[i];
		GPIOB->MODER &= ~(0x03 << (2 * keyB2));
		GPIOB->OTYPER &= ~(0x01 << keyB2);
		GPIOB->PUPDR &= ~(0x03 << (2 * keyB2));

		// output for columns
		int keyB = keyPadIB[i];
		GPIOB->MODER &= ~(0x03 << (2 * keyB));
		GPIOB->MODER |= (0x01 << (2 * keyB));
		GPIOB->OTYPER &= ~(0x01 << keyB);
		GPIOB->PUPDR &= ~(0x03 << (2 * keyB));
	}
}
void subscribeSwitch()
{
	if (Events.switchsubcribed != 0)
		return;
	Events.switchsubcribed = 1;
	for (int i = 0; i < 4; i++)
	{
		int keyb2 = keyPadIB2[i];
		GPIOA->MODER &= ~(0x03 << (2 * (i + 8)));
		GPIOA->MODER |= (0x01 << (2 * (i + 8)));
		GPIOA->OTYPER &= ~(0x01 << (i + 8));
		GPIOA->PUPDR &= ~(0x03 << (2 * (i + 8)));
	}
}

void onKeyPadPress(void (*eventHandler)(enum KEYPAD key))
{
	subscribeKeyPad();
	int size = length(Events.onKeyPadPressCallbacks);
	void (**arrayNew)(enum KEYPAD key) = malloc((size + 2) * sizeof(void (*)(enum KEYPAD key)));
	for (int i = 0; i < size; i++)
	{
		arrayNew[i] = Events.onKeyPadPressCallbacks[i];
	}
	arrayNew[size] = eventHandler;
	arrayNew[size + 1] = NULL;
	free(Events.onKeyPadPressCallbacks);
	Events.onKeyPadPressCallbacks = arrayNew;
}
void onKeyPadRelease(void (*eventHandler)(enum KEYPAD key))
{
	subscribeKeyPad();
	int size = length(Events.onKeyPadReleaseCallbacks);
	void (**arrayNew)(enum KEYPAD key) = malloc((size + 2) * sizeof(void (*)(enum KEYPAD key)));
	for (int i = 0; i < size; i++)
	{
		arrayNew[i] = Events.onKeyPadReleaseCallbacks[i];
	}
	arrayNew[size] = eventHandler;
	arrayNew[size + 1] = NULL;
	free(Events.onKeyPadReleaseCallbacks);
	Events.onKeyPadReleaseCallbacks = arrayNew;
}
void onSwitchRelease(void (*eventHandler)(enum SWITCHS sw))
{
	subscribeSwitch();
	int size = length(Events.onSwitchReleaseCallbacks);
	void (**arrayNew)(enum SWITCHS sw) = malloc((size + 2) * sizeof(void (*)(enum SWITCHS sw)));
	for (int i = 0; i < size; i++)
	{
		arrayNew[i] = Events.onSwitchReleaseCallbacks[i];
	}
	arrayNew[size] = eventHandler;
	arrayNew[size + 1] = NULL;
	free(Events.onSwitchReleaseCallbacks);
	Events.onSwitchReleaseCallbacks = arrayNew;
}
void onSwitchPress(void (*eventHandler)(enum SWITCHS sw))
{
	subscribeSwitch();
	int size = length(Events.onSwitchPressCallbacks);
	void (**arrayNew)(enum SWITCHS sw) = malloc((size + 2) * sizeof(void (*)(enum SWITCHS sw)));
	for (int i = 0; i < size; i++)
	{
		arrayNew[i] = Events.onSwitchPressCallbacks[i];
	}
	arrayNew[size] = eventHandler;
	arrayNew[size + 1] = NULL;
	free(Events.onSwitchPressCallbacks);
	Events.onSwitchPressCallbacks = arrayNew;
}

void InitEvents()
{

	Events.onKeyPadPressCallbacks = malloc(sizeof(void (*)(enum KEYPAD key)));
	Events.onKeyPadReleaseCallbacks = malloc(sizeof(void (*)(enum KEYPAD key)));
	Events.onSwitchReleaseCallbacks = malloc(sizeof(void (*)(enum SWITCHS sw)));
	Events.onSwitchPressCallbacks = malloc(sizeof(void (*)(enum SWITCHS sw)));
	Events.onKeyPadPressCallbacks[0] = NULL;
	Events.onKeyPadReleaseCallbacks[0] = NULL;
	Events.onSwitchReleaseCallbacks[0] = NULL;
	Events.onSwitchPressCallbacks[0] = NULL;
	Events.onKeyPadPress = onKeyPadPress;
	Events.onKeyPadRelease = onKeyPadRelease;
	Events.onSwitchRelease = onSwitchRelease;
	Events.onSwitchPress = onSwitchPress;
	Events.keypadsubcribed = 0;
	Events.switchsubcribed = 0;
	Events.countToUnbounce = 30;
	for (int b = 0; b < 4; b++)
	{
		Events.switchPressTicks[b] = 0;
		Events.switchPressed[b] = 0;
		for (int c = 0; c < 4; c++)
		{
			Events.keyPadPressTicks[b][c] = 0;
			Events.keyPadPressed[b][c] = 0;
		}
	}
}
int length(void **array)
{
	int i = 0;
	while (array[i] != NULL)
	{
		i++;
	}
	return i;
}

void check()
{
	int switchPressed = 0;

	GPIOB->ODR &= ~(1 << 1);
	GPIOB->ODR &= ~(1 << 2);
	GPIOB->ODR &= ~(1 << 3);
	GPIOB->ODR &= ~(1 << 4);
	for (int row = 0; row < 4; row++)
	{
		int pressedCurrent = GPIOB->IDR & (0x1 << (11 - row));
		switchPressed += pressedCurrent;
		if (!Events.switchsubcribed)
			continue;
		if (!pressedCurrent)
		{
			if (!Events.keyPadPressed[row])
				continue;

			Events.switchPressTicks[row] = 0;
			enum SWITCHS key = SWITCH_ARRAY[row];
			int callbacksLength = length(Events.onSwitchReleaseCallbacks);
			for (int i = 0; i < callbacksLength; i++)
			{
				Events.onSwitchReleaseCallbacks[i](key);
			}
			Events.switchPressed[row] = 0;
			continue;
		}
		enum SWITCHS key = SWITCH_ARRAY[row];
		int pressed = Events.switchPressed[row];

		if (pressed)
			continue;
		int ticks = Events.switchPressTicks[row]++;
		if (ticks < Events.countToUnbounce)
			continue;

		Events.switchPressed[row] = 1;
		int callbacksLength = length(Events.onSwitchPressCallbacks);
		for (int i = 0; i < callbacksLength; i++)
		{
			Events.onSwitchPressCallbacks[i](key);
		}
	}

	if (switchPressed || !Events.keypadsubcribed)
		return;
	for (int col = 0; col < 4; col++)
	{
		GPIOB->ODR &= ~(1 << 1);
		GPIOB->ODR &= ~(1 << 2);
		GPIOB->ODR &= ~(1 << 3);
		GPIOB->ODR &= ~(1 << 4);

		uint32_t temp = GPIOB->ODR;
		GPIOB->ODR |= (1 << (1 + col));

		temp = GPIOB->ODR;
		GPIOB->ODR = temp;
		for (int row = 0; row < 4; row++)
		{

			int pressedCurrent = (GPIOB->IDR & (0x1 << (8 + row)));
			if (!pressedCurrent)
			{

				Events.keyPadPressTicks[row][col] = 0;

				if (Events.keyPadPressed[row][col])
				{

					enum KEYPAD key = KEYPAD_MATRIX[row][col];
					int callbacksLength = length(Events.onKeyPadReleaseCallbacks);
					for (int i = 0; i < callbacksLength; i++)
					{
						Events.onKeyPadReleaseCallbacks[i](key);
					}
				}
				Events.keyPadPressed[row][col] = 0;
				continue;
			}
			enum KEYPAD key = KEYPAD_MATRIX[row][col];
			int pressed = Events.keyPadPressed[row][col];

			if (pressed)
				continue;
			int ticks = Events.keyPadPressTicks[row][col]++;
			if (ticks < Events.countToUnbounce)
				continue;

			Events.keyPadPressed[row][col] = 1;
			int callbacksLength = length(Events.onKeyPadPressCallbacks);
			for (int i = 0; i < callbacksLength; i++)
			{
				Events.onKeyPadPressCallbacks[i](key);
			}
		}
	}
}