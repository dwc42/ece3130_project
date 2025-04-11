#ifndef __LIST_H
#define __LIST_H
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
extern struct Press PressVoid;
extern struct Sample SampleVoid;
int lengthPress(struct Press *list)
{
	int len = 0;
	if (list == NULL)
		return 0; // handle null pointer case
	for (; list[len].pressDate != PressVoid.pressDate; len++)
	{
	}
	return len;
}
int lengthSample(struct Sample *list)
{
	int len = 0;
	if (list == NULL)
		return 0; // handle null pointer case
	for (; list[len].frequency != SampleVoid.frequency; len++)
	{
	}
	return len;
}
void pushPress(struct Press **list, struct Press item)
{
	if (*list == NULL)
	{
		*list = malloc(sizeof(struct Press) * 2); // allocate space for one integer and INT32_MAX terminator
		(*list)[0] = item;
		(*list)[1] = PressVoid; // set the terminator
		return;
	}
	int len = lengthPress(*list);
	struct Press *new_list = malloc((sizeof(struct Press) * (len + 2)));
	for (int i = 0; i < len; i++)
	{
		new_list[i] = (*list)[i];
	}
	new_list[len] = item;
	new_list[len + 1] = PressVoid; // set the terminator
	free(*list);
	*list = new_list;
}
void pushSample(struct Sample **list, struct Sample item)
{
	if (*list == NULL)
	{
		*list = malloc(sizeof(struct Sample) * 2); // allocate space for one integer and INT32_MAX terminator
		(*list)[0] = item;
		(*list)[1] = SampleVoid; // set the terminator
		return;
	}
	int len = lengthSample(*list);
	struct Sample *new_list = malloc((sizeof(struct Sample) * (len + 2)));
	for (int i = 0; i < len; i++)
	{
		new_list[i] = (*list)[i];
	}
	new_list[len] = item;
	new_list[len + 1] = SampleVoid; // set the terminator
	free(*list);
	*list = new_list;
}
int *indexOfLengthPress(struct Press *list, struct Press item)
{
	int i = 0;
	int index = -1;
	for (; list[i].pressDate != PressVoid.pressDate; i++)
	{
		if (index <= -1 && list[i].pressDate == item.pressDate && list[i].RecordingIndex == item.RecordingIndex) // check for equality or within precision range
		{
			index = i; // found the first occurrence of the value
		}
	}
	int *out = malloc(sizeof(int) * 2);
	out[0] = index; // index of the value or -1 if not found
	out[1] = i;		// length of the list
	return out;
}
void removeFromPresses(struct Press **list, struct Press value)
{
	if (*list == NULL)
		return; // handle null pointer case

	int *dat = indexOfLengthPress(*list, value);
	int indexFound = dat[0];
	if (indexFound <= -1)
	{
		free(dat);
		return; // value not found, nothing to remove
	}
	int len = dat[1];
	free(dat);

	// Create a new list without the found value
	struct Press *new_list = malloc(sizeof(struct Press) * (len)); // +1 for INT32_MAX terminator
	int j = 0;
	for (int i = 0; i < len; i++)
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

#endif /*__LIST_H*/