#include "mytest.h"
#include <math.h>

int add(int x, int y)
{
	return (x + y);
}

int sub(int x, int y)
{
	return (x - y);
}

float mysqrt(float x)
{
	if(x <= 0)
		return 0;
	return sqrt(x);	
}
