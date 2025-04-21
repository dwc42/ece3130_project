#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdint.h>
#include "list.h"
int lengthIntegers(int *list)
{
	int len = 0;
	if (list == NULL)
		return 0; // handle null pointer case
	for (; list[len] != INT32_MAX; len++)
	{
	}
	return len;
}
void pushInteger(int **list, int item)
{
	if (*list == NULL)
	{
		*list = malloc(sizeof(int) * 2); // allocate space for one integer and INT32_MAX terminator
		(*list)[0] = item;
		(*list)[1] = INT32_MAX; // set the terminator
		return;
	}
	int len = lengthIntegers(*list);
	int *new_list = malloc((sizeof(int) * (len) + 1) + sizeof(INT32_MAX));
	for (int i = 0; i < len; i++)
	{
		new_list[i] = (*list)[i];
	}
	new_list[len] = item;
	new_list[len + 1] = INT32_MAX; // set the terminator
	free(*list);
	*list = new_list;
}
int *indexOfLengthInteger(int *list, int value, int precision)
{
	int i = 0;
	int index = -1;
	for (; list[i] != INT32_MAX; i++)
	{
		if (index <= -1 && abs(list[i] - value) <= precision) // check for equality or within precision range
		{
			index = i; // found the first occurrence of the value
		}
	}
	int *out = malloc(sizeof(int) * 2);
	out[0] = index; // index of the value or -1 if not found
	out[1] = i;		// length of the list
	return out;
}
void removeFromIntegers(int **list, int value, int precision)
{
	if (*list == NULL)
		return; // handle null pointer case

	int *dat = indexOfLengthInteger(*list, value, precision);
	int indexFound = dat[0];
	if (indexFound <= -1)
	{
		free(dat);
		return; // value not found, nothing to remove
	}
	int len = dat[1];
	free(dat);

	// Create a new list without the found value
	int *new_list = malloc(sizeof(int) * (len - 1) + sizeof(int) * 2); // +1 for INT32_MAX terminator
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (i != indexFound)
		{
			new_list[j++] = (*list)[i];
		}
	}
	new_list[j] = INT32_MAX; // set the terminator

	free(*list);
	*list = new_list;
}

struct Press PressVoid = {0, 0};
struct Sample SampleVoid = {0, 0, 0};
uint32_t lengthPress(struct Press *list)
{
	uint32_t len = 0;
	if (list == NULL)
		return 0; // handle null pouint32_ter case
	for (; list[len].pressDate != PressVoid.pressDate; len++)
	{
	}
	return len;
}
uint32_t lengthSample(struct Sample *list)
{
	uint32_t len = 0;
	if (list == NULL)
		return 0; // handle null pouint32_ter case
	for (; list[len].frequency != SampleVoid.frequency; len++)
	{
	}
	return len;
}
uint32_t pushPress(struct Press **list, struct Press item)
{
	if (*list == NULL)
	{
		*list = malloc(sizeof(struct Press) * 2); // allocate space for one uint32_teger and uint32_t32_MAX terminator
		(*list)[0] = item;
		(*list)[1] = PressVoid; // set the terminator
		return 0;
	}
	uint32_t len = lengthPress(*list);
	struct Press *new_list = malloc((sizeof(struct Press) * (len + 2)));
	for (uint32_t i = 0; i < len; i++)
	{
		new_list[i] = (*list)[i];
	}
	new_list[len] = item;
	new_list[len + 1] = PressVoid; // set the terminator
	free(*list);
	*list = new_list;
	return len;
}
int pushSample(struct Sample **list, struct Sample item)
{
	if (*list == NULL)
	{
		*list = malloc(sizeof(struct Sample) * 2); // allocate space for one uint32_teger and uint32_t32_MAX terminator
		(*list)[0] = item;
		(*list)[1] = SampleVoid; // set the terminator
		return 0;
	}
	uint32_t len = lengthSample(*list);
	struct Sample *new_list = malloc((sizeof(struct Sample) * (len + 2)));
	for (uint32_t i = 0; i < len; i++)
	{
		new_list[i] = (*list)[i];
	}
	new_list[len] = item;
	new_list[len + 1] = SampleVoid; // set the terminator
	free(*list);
	*list = new_list;
	return len;
}
int *indexOfLengthPress(struct Press *list, struct Press item)
{
	uint32_t i = 0;
	uint32_t index = -1;
	for (; list[i].pressDate != PressVoid.pressDate; i++)
	{
		if (index <= -1 && list[i].pressDate == item.pressDate && list[i].RecordingIndex == item.RecordingIndex) // check for equality or within precision range
		{
			index = i; // found the first occurrence of the value
		}
	}
	uint32_t *out = malloc(sizeof(uint32_t) * 2);
	out[0] = index; // index of the value or -1 if not found
	out[1] = i;		// length of the list
	return out;
}
void removeFromPresses(struct Press **list, struct Press value)
{
	if (*list == NULL)
		return; // handle null pouint32_ter case

	uint32_t *dat = indexOfLengthPress(*list, value);
	uint32_t indexFound = dat[0];
	if (indexFound <= -1)
	{
		free(dat);
		return; // value not found, nothing to remove
	}
	uint32_t len = dat[1];
	free(dat);

	// Create a new list without the found value
	struct Press *new_list = malloc(sizeof(struct Press) * (len)); // +1 for uint32_t32_MAX terminator
	uint32_t j = 0;
	for (uint32_t i = 0; i < len; i++)
	{
		if (i != indexFound)
		{
			new_list[j++] = (*list)[i];
		}
	}
	new_list[j] = PressVoid; // set the terminator

	free(*list);
	*list = new_list;
}
//
//

//
//
struct Play PlayVoid = {0, 0};
uint32_t lengthPlay(struct Play *list)
{
	uint32_t len = 0;
	if (list == NULL)
		return 0; // handle null pouint32_ter case
	for (; list[len].frequency != PlayVoid.frequency; len++)
	{
	}
	return len;
}
int pushPlay(struct Play **list, struct Play item)
{
	if (*list == NULL)
	{
		*list = malloc(sizeof(struct Play) * 2); // allocate space for one uint32_teger and uint32_t32_MAX terminator
		(*list)[0] = item;
		(*list)[1] = PlayVoid; // set the terminator
		return 0;
	}
	uint32_t len = lengthPlay(*list);
	struct Play *new_list = malloc((sizeof(struct Play) * (len + 2)));
	for (uint32_t i = 0; i < len; i++)
	{
		new_list[i] = (*list)[i];
	}
	new_list[len] = item;
	new_list[len + 1] = PlayVoid; // set the terminator
	free(*list);
	*list = new_list;
	return len;
}
int *indexOfLengthPlay(struct Play *list, struct Play item)
{
	uint32_t i = 0;
	int index = -1;
	for (; list[i].frequency != PlayVoid.frequency; i++)
	{
		if (index <= -1 && list[i].endDate == item.endDate && list[i].frequency == item.frequency) // check for equality or within precision range
		{
			index = i; // found the first occurrence of the value
		}
	}
	int *out = malloc(sizeof(int) * 2);
	out[0] = index; // index of the value or -1 if not found
	out[1] = i;		// length of the list
	return out;
}
void removeFromPlays(struct Play **list, struct Play value)
{
	if (*list == NULL)
		return; // handle null pouint32_ter case

	int *dat = indexOfLengthPlay(*list, value);
	int indexFound = dat[0];
	if (indexFound <= -1)
	{
		free(dat);
		return; // value not found, nothing to remove
	}
	int len = dat[1];
	free(dat);

	// Create a new list without the found value
	struct Play *new_list = malloc(sizeof(struct Play) * (len)); // +1 for uint32_t32_MAX terminator
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (i != indexFound)
		{
			new_list[j++] = (*list)[i];
		}
	}
	new_list[j] = PlayVoid; // set the terminator

	free(*list);
	*list = new_list;
}
