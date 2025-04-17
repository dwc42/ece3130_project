#include "stm32l4xx_hal.h"
#include "events.h"
#include "date.h"
#include "lcd.h"
#include "list.h"
#include "stdio.h"
#include "math.h"
struct Sample **ListSample;
void init()
{
    ListSample = malloc(sizeof(struct Sample));
    ListSample[0] = SampleVoid;
}
void recordMusicPress()
{
    struct Sample *sample = malloc(sizeof(struct Sample));
    sample->timeSinceFirstPressStart = pushSample(&ListSample, *sample);
};

void recordMusicRelease() {

};