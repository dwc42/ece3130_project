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
int lengthDoubles(double *list);
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end
 * @param item the double to add to the list
 */
void pushDouble(double *list, double item);
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end
 * @param value the double to find in the list
 * @param precision the precision to find the value
 * @return an array of two integers, the first is the index of the value in the list, the second is the length of the list
 */
int *indexOfLengthDouble(double *list, float value, float precision);
/**
 * @brief
 * @param list point to an array of doubles with a DBL_MAX at the end (pass with a & before list like &list)
 * @param value the double to remove from the list
 * @param precision the precision to find the value
 */
void removeFromDoubles(double **list, float value, float precision);
#endif /*__LIST_H*/