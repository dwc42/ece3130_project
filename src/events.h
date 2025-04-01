#ifndef __EVENTS_H
#define __EVENTS_H
#include <stdint.h>
#include <stdlib.h>
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
struct EventsType
{
	int countToUnbounce;
	int keypadsubcribed;
	int switchsubcribed;
	int keyPadPressTicks[4][4];
	int keyPadPressed[4][4];
	int switchPressTicks[4];
	int switchPressed[4];
	void (*onKeyPadPress)(void (*eventHandler)(enum KEYPAD key));
	void (*onKeyPadRelease)(void (*eventHandler)(enum KEYPAD key));
	void (*onSwitchRelease)(void (*eventHandler)(enum SWITCHS sw));
	void (*onSwitchPress)(void (*eventHandler)(enum SWITCHS sw));
	void (**onKeyPadPressCallbacks)(enum KEYPAD key);
	void (**onKeyPadReleaseCallbacks)(enum KEYPAD key);
	void (**onSwitchReleaseCallbacks)(enum SWITCHS sw);
	void (**onSwitchPressCallbacks)(enum SWITCHS sw);
};
extern char KEYPAD_CHARS[16];
void Delay(unsigned int n);
extern struct EventsType Events;
void InitEvents();
int length(void **array);
extern enum KEYPAD KEYPAD_MATRIX[4][4];
double check();
#endif /* __EVENTS_H */