#ifndef __EVENTS_H
#define __EVENTS_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "events.h"
#include "main.h"

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
enum SWITCHS
{
	BUTTON_SWITCH2 = 0,
	BUTTON_SWITCH3 = 1,
	BUTTON_SWITCH4 = 2,
	BUTTON_SWITCH5 = 3,
};
struct BeforeCharWriteEventType
{
	char c;
	bool cancel;	  // if set to 1, the event will be cancelled and not written to the output
	uint8_t position; //(readonly)
	uint8_t line;	  //(readonly)
	char string[33];  // 32 characters + null terminator (readonly)
};
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
void runBeforeCharWriteCallbacks(struct BeforeCharWriteEventType *event);
extern char KEYPAD_CHARS[16];
void Delay(unsigned int n);
extern struct EventsType Events;
void InitEvents();
int length(void **array);
extern enum KEYPAD KEYPAD_MATRIX[4][4];
double check();
#endif /* __EVENTS_H */