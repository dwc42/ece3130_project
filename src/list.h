#ifndef __LIST_H
#define __LIST_H
#include <stdint.h>
#include "config.h"
/**
 * @brief Gets the length of a list of integers, the list must be terminated with a INT_MAX
 * @note The list must be terminated with a INT_MAX, this function will not check for this
 */
int lengthIntegers(int *list);
/**
 * @brief Pushes an integer to the end of a list of integers, the list must be terminated with a INT_MAX
 * @note The list must be terminated with a INT_MAX, this function will not check for this
 * @param list address to an array of integers with a INT_MAX at the end
 * @param item the integer to be pushed to the list
 */
void pushInteger(int **list, int item);
/**
 * @brief Gets the index of a value in a list of integers, the list must be terminated with a INT_MAX
 * @note The list must be terminated with a INT_MAX, this function will not check for this
 * @param list address to an array of integers with a INT_MAX at the end
 * @param value the value to be searched for in the list
 * @param precision the precision to be used for the search, if the value is within this range, it will be considered equal
 * @return an array of two integers, the first is the index of the value in the list, the second is the length of the list
 */
int *indexOfLengthInteger(int *list, int value, int precision);
/**
 * @brief Removes a value from a list of integers, the list must be terminated with a INT_MAX
 * @note The list must be terminated with a INT_MAX, this function will not check for this
 * @param list address to an array of integers with a INT_MAX at the end
 * @param value the value to be removed from the list
 * @param precision the precision to be used for the search, if the value is within this range, it will be considered equal
 */
void removeFromIntegers(int **list, int value, int precision);
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end
 * @return the length of the list
 */

struct Press
{
	int pressDate;
	int RecordingIndex;
};
struct Sample
{
	int timeSinceFirstPressStart;
	int timeSinceFirstPressEnd;
	int frequency;
};
struct Play
{
	uint32_t endDate;
	int frequency;
};
extern struct Press PressVoid;
extern struct Sample SampleVoid;
extern struct Play PlayVoid;
// int lengthPress(struct Press *list);
uint32_t lengthPlays(struct Play list[MAX_PLAYS]);
uint32_t lengthSample(struct Sample *list);
// int pushPress(struct Press **list, struct Press item);
int pushSample(struct Sample list[MAX_SAMPLES], struct Sample item);
// int *indexOfLengthPress(struct Press *list, struct Press item);
// void removeFromPresses(struct Press **list, struct Press value);
int pushPlay(struct Play list[MAX_PLAYS], struct Play item);
int *indexOfLengthPlay(struct Play list[MAX_PLAYS], struct Play item);
void removeFromPlays(struct Play list[MAX_PLAYS], struct Play value);
#endif /*__LIST_H*/