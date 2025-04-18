#include "stm32l4xx_hal.h"
#include "events.h"
#include "date.h"
#include "lcd.h"
#include "list.h"
#include "stdio.h"
#include "math.h"
#include "run.h"

struct Sample *ListSample;
struct Press ListPress[16];

void init()
{
    ListSample = malloc(sizeof(struct Sample));
    ListSample[0] = SampleVoid;
    ListSample = malloc(sizeof(struct Sample));
    for (int i = 0; i < 16; i++)
    {
        ListPress[i] = PressVoid;
    }
}

void recordMusicPress(enum KEYPAD key)
{
    struct Sample *sample = malloc(sizeof(struct Sample));
    struct Press *press = malloc(sizeof(struct Press));
    int frequency = Frequencies[presetIndex][key];
    int sampleIndex = pushSample(&ListSample, *sample);
    press->RecordingIndex = sampleIndex;
    press->pressDate = date();
    ListSample[sampleIndex] = *sample;
};

void recordMusicRelease()
{
    frequency = Frequencies[presetIndex][key];

    struct Sample *sample = malloc(sizeof(struct Sample));
};