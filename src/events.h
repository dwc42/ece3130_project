#ifndef __EVENTS_H
#define __EVENTS_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "events.h"
#include "main.h"
/**
 * @brief Enum of Keypad button keys
 */
enum KEYPAD
{
	KEY_0 = 13,
	KEY_1 = 0,
	KEY_2 = 1,
	KEY_3 = 2,
	KEY_4 = 4,
	KEY_5 = 5,
	KEY_6 = 6,
	KEY_7 = 8,
	KEY_8 = 9,
	KEY_9 = 10,
	KEY_A = 3,
	KEY_B = 7,
	KEY_C = 11,
	KEY_D = 15,
	KEY_ASTRISK = 12,
	KEY_HASH = 14,
};
/**
 * @brief Enum of Button Switches
 */
enum SWITCHS
{
	BUTTON_SWITCH2 = 0,
	BUTTON_SWITCH3 = 1,
	BUTTON_SWITCH4 = 2,
	BUTTON_SWITCH5 = 3,
};
/**
 * @brief Structure for BeforeCharWriteEventType
 * @struct BeforeCharWriteEventType
 * @note c: the character that will be written to the output
 * @note cancel: default is 0; if set to 1, the event will be cancelled and not written to the lcd
 * @note position(readonly/copy): the current position of the cursor in the string (0-31)
 * @note line(readonly/copy): the current line of the lcd (0-1)
 * @note string(readonly/copy): the string that will be written to the lcd, 32 characters + null terminator
 *
 */
struct BeforeCharWriteEventType
{
	char c;
	bool cancel;	  // if set to 1, the event will be cancelled and not written to the output
	uint8_t position; //(readonly)
	uint8_t line;	  //(readonly)
	char string[33];  // 32 characters + null terminator (readonly)
};
/**
 * @brief Structure for EventsType
 * @struct EventsType
 * @note countToUnbounce: the number of ticks to wait before considering a button press as valid
 * @note keypadsubcribed: if the keypad is subscribed or not, used to run callbacks for keypad events
 * @note switchsubcribed: if the switch is subscribed or not, used to run callbacks for switch events
 * @note beforeCharWriteSubscribed: if before char write is subscribed or not, used to run callbacks for before char write events
 * @note keyPadPressTicks: the number of ticks since the last key press for each key in the keypad
 * @note keyPadPressed: if the key is pressed or not for each key in the keypad
 * @note switchPressTicks: the number of ticks since the last switch press for each switch
 * @note switchPressed: if the switch is pressed or not for each switch
 * @note onKeyPadPress: function pointer for the onKeyPadPress event
 * @note onKeyPadRelease: function pointer for the onKeyPadRelease event
 * @note onSwitchRelease: function pointer for the onSwitchRelease event
 * @note onSwitchPress: function pointer for the onSwitchPress event
 * @note unsubscribeEvent: function pointer for the unsubscribeEvent event
 * @note beforeCharWrite: function pointer for the beforeCharWrite event
 * @note beforeCharWriteCallbacks: function pointer for the beforeCharWrite event callbacks
 * @note onKeyPadPressCallbacks: function pointer for the onKeyPadPress event callbacks
 * @note onKeyPadReleaseCallbacks: function pointer for the onKeyPadRelease event callbacks
 * @note onSwitchReleaseCallbacks: function pointer for the onSwitchRelease event callbacks
 * @note onSwitchPressCallbacks: function pointer for the onSwitchPress event callbacks
 */
struct EventsType
{
	uint8_t countToUnbounce;
	uint8_t keypadsubcribed;
	uint8_t switchsubcribed;
	uint8_t beforeCharWriteSubscribed; // if before char write is subscribed or not, used to run callbacks for before char write events
	uint8_t keyPadPressTicks[4][4];
	uint8_t keyPadPressed[4][4];
	uint8_t switchPressTicks[4];
	uint8_t switchPressed[4];
	void (*onKeyPadPress)(void (*eventHandler)(enum KEYPAD key));
	void (*onKeyPadRelease)(void (*eventHandler)(enum KEYPAD key));
	void (*onSwitchRelease)(void (*eventHandler)(enum SWITCHS sw));
	void (*onSwitchPress)(void (*eventHandler)(enum SWITCHS sw));
	void (*unsubscribeEvent)(void *eventHandler);
	void (*beforeCharWrite)(void (*eventHandler)(struct BeforeCharWriteEventType *event));
	void (**beforeCharWriteCallbacks)(struct BeforeCharWriteEventType *event); // Callbacks for before character write events, used in UART or other output streams
	void (**onKeyPadPressCallbacks)(enum KEYPAD key);
	void (**onKeyPadReleaseCallbacks)(enum KEYPAD key);
	void (**onSwitchReleaseCallbacks)(enum SWITCHS sw);
	void (**onSwitchPressCallbacks)(enum SWITCHS sw);
};
/**
 * @brief Runs the callbacks for the before char write event
 * @param event the event type that stores the event data
 * @note c: the character that will be written to the output
 * @note cancel: default is 0; if set to 1, the event will be cancelled and not written to the lcd
 * @note position(readonly/copy): the current position of the cursor in the string (0-31)
 * @note line(readonly/copy): the current line of the lcd (0-1)
 * @note string(readonly/copy): the string that will be written to the lcd, 32 characters + null terminator
 */
void runBeforeCharWriteCallbacks(struct BeforeCharWriteEventType *event);
/**
 * Keypad matrix for the keypad
 * @note 4x4 matrix for the keypad, used to map the keys to their corresponding values
 */
extern char KEYPAD_CHARS[16];
/**
 * @brief Wrapper function for the delay function
 * @param n the number of milliseconds to delay
 */
void Delay(unsigned int n);
/**
 * @brief Structure for EventsType
 * @struct EventsType
 * @note onKeyPadPress: function pointer for the onKeyPadPress event
 * @note parameter: enum KEYPAD key pressed
 * @note ____
 * @note onKeyPadRelease: function pointer for the onKeyPadRelease event
 * @note -parameter: enum KEYPAD key released
 * @note ____
 * @note onSwitchRelease: function pointer for the onSwitchRelease event
 * @note -parameter: enum SWITCHS key pressed
 * @note ____
 * @note onSwitchPress: function pointer for the onSwitchPress event
 * @note -parameter: enum SWITCHS key released
 * @note ____
 * @note beforeCharWrite: function pointer for the beforeCharWrite event
 * @note -parameter: struct BeforeCharWriteEventType event
 * @note --c: the character that will be written to the output
 * @note --cancel: default is 0; if set to 1, the event will be cancelled and not written to the lcd
 * @note --position(readonly/copy): the current position of the cursor in the string (0-31)
 * @note --line(readonly/copy): the current line of the lcd (0-1)
 * @note --string(readonly/copy): the string that will be written to the lcd, 32 characters + null terminator
 */
extern struct EventsType Events;
/**
 * @brief Initializes the events structure and sets the default values
 */
void InitEvents();
/**
 * length: returns the length of the array
 * @param array the array to get the length of
 */
int length(void **array);
/**
 * @brief KEYPAD_MATRIX is a 4x4 matrix that maps the keys to their corresponding values
 * @note 0-3: first row, 4-7: second row, 8-11: third row, 12-15: fourth row
 */
extern enum KEYPAD KEYPAD_MATRIX[4][4];
/**
 * @brief function to poll the keypad and switch events
 */
double check();
#endif /* __EVENTS_H */