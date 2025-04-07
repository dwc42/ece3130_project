#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "events.h"
#include "stm32l4xx_hal.h"

struct EventsType Events;
void Delay(unsigned int n)
{
	HAL_Delay(n);
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

void beforeCharWrite(void (*eventHandler)(struct BeforeCharWriteEventType *event))
{
	int size = length(Events.beforeCharWriteCallbacks);
	void (**arrayNew)(struct BeforeCharWriteEventType *event) = malloc((size + 2) * sizeof(void (*)(struct BeforeCharWriteEventType *event)));
	for (int i = 0; i < size; i++)
	{
		arrayNew[i] = Events.beforeCharWriteCallbacks[i];
	}
	arrayNew[size] = eventHandler;
	arrayNew[size + 1] = NULL;
	free(Events.beforeCharWriteCallbacks);
	Events.beforeCharWriteSubscribed = 1;
	Events.beforeCharWriteCallbacks = arrayNew;
}
void runBeforeCharWriteCallbacks(struct BeforeCharWriteEventType *event)
{

	if (Events.beforeCharWriteCallbacks == NULL)
		return;
	int size = length(Events.beforeCharWriteCallbacks);
	for (int i = 0; i < size; i++)
	{
		if (Events.beforeCharWriteCallbacks[i] != NULL)
		{
			Events.beforeCharWriteCallbacks[i](event);
		}
	}
}
int removeAddressFromEvent(void *address, void (**callbackArray)(void), int size)
{
	if (callbackArray == NULL || address == NULL)
		return 0;
	void (**arrayNew)(void) = malloc(size * sizeof(void (*)(void)));
	int j = 0;
	int found = 0; // flag to check if we found the address in the array
	for (int i = 0; callbackArray[i] != '\0'; i++)
	{
		if ((void *)callbackArray[i] == (void *)address)
		{
			found = 1;
			continue; // skip the address we want to remove
		}
		arrayNew[j++] = callbackArray[i];
	}
	arrayNew[j] = NULL; // null terminate the array
	free(callbackArray);
	callbackArray = arrayNew;
	return found;
}
void unsubscribeEvent(void *address)
{
	if (!removeAddressFromEvent(
			address,
			(void (**)(void))Events.onKeyPadPressCallbacks,
			length(Events.onKeyPadPressCallbacks)))
	{
	}
	if (!removeAddressFromEvent(
			address,
			(void (**)(void))Events.onKeyPadReleaseCallbacks,
			length(Events.onKeyPadReleaseCallbacks)))
	{
	}
	if (!removeAddressFromEvent(
			address,
			(void (**)(void))Events.onSwitchReleaseCallbacks,
			length(Events.onSwitchReleaseCallbacks)))
	{
	}
	if (!removeAddressFromEvent(

			address,
			(void (**)(void))Events.onSwitchPressCallbacks,
			length(Events.onSwitchPressCallbacks)))
	{
	}
	if (!removeAddressFromEvent(
			address,
			(void (**)(void))Events.beforeCharWriteCallbacks,
			length(Events.beforeCharWriteCallbacks)))
	{
	}
}
void InitEvents()
{

	Events.onKeyPadPressCallbacks = malloc(sizeof(void (*)(enum KEYPAD key)));
	Events.onKeyPadReleaseCallbacks = malloc(sizeof(void (*)(enum KEYPAD key)));
	Events.onSwitchReleaseCallbacks = malloc(sizeof(void (*)(enum SWITCHS sw)));
	Events.onSwitchPressCallbacks = malloc(sizeof(void (*)(enum SWITCHS sw)));
	Events.beforeCharWriteCallbacks = malloc(sizeof(void (*)(struct BeforeCharWriteEventType *event)));
	Events.onKeyPadPressCallbacks[0] = NULL;
	Events.onKeyPadReleaseCallbacks[0] = NULL;
	Events.onSwitchReleaseCallbacks[0] = NULL;
	Events.onSwitchPressCallbacks[0] = NULL;
	Events.beforeCharWriteCallbacks[0] = NULL;
	Events.onKeyPadPress = onKeyPadPress;
	Events.onKeyPadRelease = onKeyPadRelease;
	Events.onSwitchRelease = onSwitchRelease;
	Events.onSwitchPress = onSwitchPress;
	Events.beforeCharWrite = beforeCharWrite;
	Events.keypadsubcribed = 0;
	Events.switchsubcribed = 0;
	Events.beforeCharWriteSubscribed = 0; // if before char write is subscribed or not, used to run callbacks for before char write events
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

double check()
{
	double timetest = 0;
	int switchPressed = 0; // used to cancel the scan of the keypad if a switch is pressed
	// clear the output data register for the rows
	GPIOB->ODR &= ~(1 << 1);
	GPIOB->ODR &= ~(1 << 2);
	GPIOB->ODR &= ~(1 << 3);
	GPIOB->ODR &= ~(1 << 4);
	// scan the rows for the switches
	for (int row = 0; row < 4; row++)
	{
		// check if Ith switch is pressed
		int pressedCurrent = GPIOB->IDR & (0x1 << (11 - row));
		switchPressed += pressedCurrent;
		if (!Events.switchsubcribed)
			continue;

		if (!pressedCurrent) // check for release event
		{
			if (!Events.keyPadPressed[row]) // check if the switch has fired the event before
				continue;

			Events.switchPressTicks[row] = 0;	  // reset the ticks for the switch
			enum SWITCHS key = SWITCH_ARRAY[row]; // get the key from the array

			// call all the callbacks for the switch release event
			int callbacksLength = length(Events.onSwitchReleaseCallbacks);
			for (int i = 0; i < callbacksLength; i++)
			{
				Events.onSwitchReleaseCallbacks[i](key);
			}
			// set the switch pressed to 0
			Events.switchPressed[row] = 0;
			continue;
		}
		enum SWITCHS key = SWITCH_ARRAY[row];	 // get the key from the array
		int pressed = Events.switchPressed[row]; // check if the switch is pressed before

		if (pressed)
			continue;
		int ticks = Events.switchPressTicks[row]++; // increment the ticks for debouncing of the switch
		if (ticks < Events.countToUnbounce)			// check if the switch is pressed for the required ticks
			continue;

		Events.switchPressed[row] = 1; // set the switch pressed to 1 in the array

		// call all the callbacks for the switch press event
		int callbacksLength = length(Events.onSwitchPressCallbacks);
		for (int i = 0; i < callbacksLength; i++)
		{
			Events.onSwitchPressCallbacks[i](key);
		}
	}

	if (switchPressed || !Events.keypadsubcribed)
		return timetest; // return if a switch is pressed or if the keypad is not subscribed

	for (int col = 0; col < 4; col++) // scan all the cols
	{

		uint32_t temp = GPIOB->ODR;
		GPIOB->ODR |= (1 << (1 + col)); // set the col to high

		temp = GPIOB->ODR;
		GPIOB->ODR = temp;
		for (int row = 0; row < 4; row++)
		{

			int pressedCurrent = (GPIOB->IDR & (0x1 << (8 + row))); // check if the row is pressed
			if (!pressedCurrent)
			{

				Events.keyPadPressTicks[row][col] = 0; // reset the ticks for the key

				if (Events.keyPadPressed[row][col]) // check if the key has fired the event before
				{

					enum KEYPAD key = KEYPAD_MATRIX[row][col]; // get the key from the array

					// call all the callbacks for the key release event
					int callbacksLength = length(Events.onKeyPadReleaseCallbacks); //
					for (int i = 0; i < callbacksLength; i++)
					{
						Events.onKeyPadReleaseCallbacks[i](key);
					}
				}
				// set the key pressed to 0 in the array
				Events.keyPadPressed[row][col] = 0;
				continue;
			}
			// check if the key is pressed
			enum KEYPAD key = KEYPAD_MATRIX[row][col];
			// check if the key is pressed before
			int pressed = Events.keyPadPressed[row][col];

			if (pressed)
				continue;
			int ticks = Events.keyPadPressTicks[row][col]++; // increment the ticks for debouncing of the key
			if (ticks < Events.countToUnbounce)
				continue; // check if the key is pressed for the required ticks

			Events.keyPadPressed[row][col] = 1; // set the key pressed to 1 in the array

			// call all the callbacks for the key press event
			int callbacksLength = length(Events.onKeyPadPressCallbacks);
			for (int i = 0; i < callbacksLength; i++)
			{
				Events.onKeyPadPressCallbacks[i](key);
			}
		}
		GPIOB->ODR &= ~(1 << (col + 1)); // clear the col to low
	}
	return timetest;
}