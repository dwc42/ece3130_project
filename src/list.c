#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdint.h>
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