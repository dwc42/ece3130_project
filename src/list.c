#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end
 * @return the length of the list
 */
int lengthDoubles(double *list)
{
	int len = 0;
	for (; list[len] < DBL_MAX; len++)
	{
	}
	return len;
}
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end
 * @param item the double to add to the list
 */
void pushDouble(double *list, double item)
{
	int len = lengthDoubles(list);
	double *new_list = malloc((sizeof(double) * (len) + 1) + sizeof('\0'));
	for (int i = 0; i < len; i++)
	{
		new_list[i] = list[i];
	}
	new_list[len] = item;
	new_list[len + 1] = DBL_MAX;
	list = new_list;
}
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end
 * @param value the double to find in the list
 * @param precision the precision to find the value
 * @return an array of two integers, the first is the index of the value in the list, the second is the length of the list
 */

int *indexOfLengthDouble(double *list, float value, float precision)

{
	int i = 0;
	int index = -1;
	for (; list[i] != DBL_MAX; i++)
	{
		if (index <= -1 && fabs(list[i] - value) < precision)
		{
			index = i;
		}
	}
	int *out = malloc(sizeof(int) * 2);
	out[0] = index;
	out[1] = i;
	return out;
}
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end (pass with a & before list like &list)
 * @param value the double to remove from the list
 * @param precision the precision to find the value
 */
void removeFromDoubles(double **list, float value, float precision)
{
	int *dat = indexOfLengthDouble(*list, value, precision);
	int indexFound = dat[0];
	if (indexFound <= -1)
	{
		return;
	}
	int len = dat[1];
	free(dat);
	int j = 0;
	double *new_list = malloc(sizeof(double) * (len - 1) + sizeof(DBL_MAX));
	for (int i = 0; i < len; i++)
	{
		if (i != indexFound)
		{
			new_list[j++] = (*list)[i];
		}
	}
	new_list[j] = DBL_MAX;
	*list = new_list;
}