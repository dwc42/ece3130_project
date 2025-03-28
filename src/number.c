#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int countSigFigs(double num, int decimalPlaces)
{
	int places = log10(num) + 1;
	int count = 0;
	if (num == 0)
		return 1;
	if (num < 0)
		count++;

	double fraction = num - (int)num; // Extract the fractional part

	for (int i = 0; i < decimalPlaces && fraction > 0; i++)
	{
		fraction *= 10;
		fraction -= (int)fraction; // Remove the integer part
		count++;
	}

	return count;
}
char *doubleToString(double num, int decimalPlaces)
{
	int sigFigs = countSigFigs(num, decimalPlaces);
	char *str = (char *)malloc(sigFigs + 1); // +1 for the null terminator
	if (!str)
		return NULL;

	sprintf(str, "%.*f", sigFigs, num);
	return str;
}