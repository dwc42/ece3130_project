#include "stm32l4xx_hal.h"
#include "events.h"
#include "date.h"
#include "lcd.h"
#include "list.h"
#include "stdio.h"
#include "math.h"
#include "buzzer.h"
#include "run.h"
#include "config.h"
#include "stdint.h"
#include "recording.h"
struct Sample ListSample[500];
struct Press ListPress[16];
uint32_t startPlayBackDate = 0;

struct Channel ChannelVoid = {NULL, 0, 0}; // Void channel to be used as a terminator or default value

struct Channel ListChannel[4]; // Array of 4 channels, each channel has its own list of samples and period
uint8_t recording = 0;
uint8_t playBack = 0;
void init()
{
    for (uint16_t i = 0; i < 500; i++)
    {
        ListSample[i] = SampleVoid; // Initialize the ListSample array with SampleVoid
        ListChannel[0].ListSample[i] = SampleVoid;
        ListChannel[1].ListSample[i] = SampleVoid;
        ListChannel[2].ListSample[i] = SampleVoid;
        ListChannel[3].ListSample[i] = SampleVoid;
    }
    for (uint8_t i = 0; i < 16; i++)
    {
        ListPress[i] = PressVoid;
    }

    ListChannel[0] = ChannelVoid;
    ListChannel[1] = ChannelVoid;
    ListChannel[2] = ChannelVoid;
    ListChannel[3] = ChannelVoid;
}
uint32_t ChannelIndexes[4] = {0, 0, 0, 0}; // Array to keep track of the current index of each channel
uint32_t ChannelStartDates[4] = {0, 0, 0, 0};

uint32_t firstPress = 0;
uint32_t timeSincePlayBackStart = 0;
uint32_t timeSinceLoop0Start = 0;
void recordMusicPress(enum KEYPAD key)
{
    if (!recording)
        return;
    uint32_t frequency = newFrequencies[presetIndex][key].frequency;
    if (!frequency)
        return;
    struct Sample *sample = malloc(sizeof(struct Sample));
    struct Press *press = malloc(sizeof(struct Press));

    sample->frequency = frequency;
    uint32_t pressDate = (uint32_t)date(); // Get the current date in milliseconds
    sample->timeSinceFirstPressStart = pressDate - firstPress;
    sample->timeSinceFirstPressEnd = 0; // Initialize to 0, will be updated on release
    uint32_t sampleIndex = pushSample(&ListSample, *sample);
    press->RecordingIndex = sampleIndex;
    press->pressDate = pressDate;
    ListPress[key] = *press; // Store the press in the ListPress array
    free(sample);
    free(press);
};

void recordMusicRelease(enum KEYPAD key)
{
    if (!recording)
        return;
    uint32_t frequency = newFrequencies[presetIndex][key].frequency;
    if (!frequency)
        return;
    uint32_t ReleaseDate = (uint32_t)date();
    struct Press press = ListPress[key]; // Get the press from the ListPress array
    if (press.pressDate == PressVoid.pressDate)
        return;
    ListSample[press.RecordingIndex].timeSinceFirstPressEnd = ReleaseDate - firstPress; // Update the time since first press end
    ListPress[key] = PressVoid;
};
void recordMusicStart()
{

    firstPress = (uint32_t)date();
    recording = 1;
    if (!ChannelStartDates[0])
        return;
    timeSinceLoop0Start = firstPress - ChannelStartDates[0]; // Initialize the start date of the first channel
    timeSincePlayBackStart = firstPress - startPlayBackDate; // Initialize the start date of playback
}
void recordMusicEnd()
{
    recording = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        if (ListChannel[i].defined)
            continue;

        uint32_t len = lengthSample(ListSample);
        if (len <= 0)
        {
            ListChannel[i].ListSample[0] = SampleVoid; // If there are no samples, set the channel to void
            continue;
        }
        for (uint32_t j = 0; j < len; j++)
        {
            ListChannel[i].ListSample[j] = ListSample[j]; // Copy the samples from the main list to the channel's list
            ListSample[j] = SampleVoid;                   // Fill the rest of the channel's list with SampleVoid
        }
        ListChannel[i].ListSample[len] = SampleVoid; // Set the last element to SampleVoid to indicate the end of the list
        ListChannel[i].defined = 1;                  // Mark the channel as defined
        ListChannel[i].peroid = ListChannel[i].ListSample[len - 1].timeSinceFirstPressEnd;
    }
}
void removeChannel(uint8_t channelIndex)
{
    for (uint8_t i = channelIndex; i < 4; i++)
    {
        int lenCurrentChannel = lengthSample(ListChannel[i].ListSample);
        if (lenCurrentChannel > 0)
        {
            for (int j = 0; j < lenCurrentChannel; j++)
            {
                ListChannel[i].ListSample[j] = SampleVoid; // Fill the channel's list with SampleVoid
            }
            continue;
        }
        ListChannel[i].defined = 0; // If the channel is empty, mark it as undefined
        ListChannel[i].peroid = 0;  // Reset the period of the channel
        struct Channel nextChannel = ListChannel[i + 1];
        int lenNextChannel = lengthSample(nextChannel.ListSample);
        if (lenNextChannel > 0)
        {
            for (int j = 0; j < lenNextChannel; j++)
            {
                ListChannel[i].ListSample[j] = nextChannel.ListSample[j]; // Copy the samples from the next channel to the current channel
            }
            ListChannel[i].ListSample[lenNextChannel] = SampleVoid; // Set the last element to SampleVoid to indicate the end of the list
            ListChannel[i].defined = true;
            // Set the defined status of the current channel to that of the next channel
            ListChannel[i].peroid = nextChannel.peroid; // Set the period of the current channel to that of the next channel
        }
    }
}
void disableChannel(uint8_t channelIndex)
{
    if (channelIndex >= 4)
    {
        return; // If the channel index is out of bounds, do nothing
    }
}

struct Sample sample;
uint32_t calculateStartDate(uint32_t channelIndex)
{
    if (channelIndex >= 4)
    {
        return 0; // If the channel index is out of bounds, return 0
    }
    uint32_t startDate = date();
    struct Channel channel = ListChannel[channelIndex];
    if (channelIndex)
    {
        if (!syncPlayback)
        {
            startDate += timeSincePlayBackStart; // If sync playback is enabled, add the time since playback started to the start date
        }
        else
        {
            startDate += timeSinceLoop0Start; // If sync playback is not enabled, add the time since loop 0 started to the start date
        }
    }

    return startDate;
}
void playRecording()
{
    if (!playBack)
    {
        return;
    }

    uint32_t currentDate = date();
    for (uint8_t i = 0; ListChannel[i].defined && i < 4; i++)
    {
        if (!ChannelStartDates[i])
        {
            ChannelStartDates[i] = calculateStartDate(i); // Calculate the start date for the channel if it is not set
            ChannelIndexes[i] = 0;                        // Reset the channel index to 0
            continue;                                     // Skip to the next channel if it is not defined
        }

        uint32_t startDate = ChannelStartDates[i];
        uint32_t channelIndex = ChannelIndexes[i];
        /*struct Sample*/ sample = ListChannel[i].ListSample[channelIndex];
        if (sample.frequency == SampleVoid.frequency)
        {
            if (channelIndex > 0)
            {
                uint32_t endDateSample = ChannelStartDates[i] + ListChannel[i].ListSample[channelIndex - 1].timeSinceFirstPressEnd;
                if (currentDate < endDateSample)
                {
                    continue; // If the current date is before the end date of the previous sample, skip to the next channel
                }
            }
            ChannelIndexes[i] = 0;
            ChannelStartDates[i] = (uint32_t)date();
            continue;
        }
        uint32_t startDateSample = sample.timeSinceFirstPressStart + startDate;
        if (currentDate < startDateSample)
        {
            continue;
        }
        uint32_t endDateSample = ChannelStartDates[i] + sample.timeSinceFirstPressEnd;

        AddFrequency(sample.frequency, endDateSample);
        ChannelIndexes[i]++;
    }
}
void toggleRecording()
{
    if (!recording)
    {
        recordMusicStart();
        return;
    }
    recordMusicEnd();
}
void togglePlayBack()
{
    if (playBack)
    {

        ChannelStartDates[0] = 0;
        ChannelStartDates[1] = 0;
        ChannelStartDates[2] = 0;
        ChannelStartDates[3] = 0;
        for (int i = 0; frequency_list[i].frequency != PlayVoid.frequency; i++)
        {
            if (frequency_list->endDate == 0)
                continue;
            removeFromPlays(frequency_list, frequency_list[i]);
            if (i > 0)
                i--;
        }
        playBack = 0;
        return;
    }
    startPlayBackDate = (uint32_t)date(); // Get the current date in milliseconds
    playBack = 1;
}