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
#include "stdint.h"
struct Channel
{
	struct Sample ListSample[500]; // Pouint32_ter to the list of samples
	uint32_t peroid;			   // The period of the channel in milliseconds
	uint8_t defined;
	uint32_t timeSinceLoop0Start;
	uint32_t timeSincePlayBackStart; // Time since the playback started
};

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