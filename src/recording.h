#ifndef __RECORDING_h
#define __RECORDING_h
#include "stm32l4xx_hal.h"
#include "events.h"
#include "date.h"
#include "lcd.h"
#include "list.h"
#include "stdio.h"
#include "math.h"
#include "buzzer.h"
#include "run.h"

void init();
void recordMusicPress(enum KEYPAD key);
void recordMusicRelease(enum KEYPAD key);
void recordMusicStart();
void recordMusicEnd();
void removeChannel(uint8_t channelIndex);
void playRecording();
void toggleRecording();
void togglePlayBack();
extern uint8_t recording;
extern uint8_t playBack;
#endif /*__MAIN_h*/