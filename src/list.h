#ifndef __LIST_H
#define __LIST_H
int lengthIntegers(int *list);
void pushInteger(int **list, int item);
int *indexOfLengthInteger(int *list, int value, int precision);
void removeFromIntegers(int **list, int value, int precision);
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end
 * @return the length of the list
 */
#endif /*__LIST_H*/