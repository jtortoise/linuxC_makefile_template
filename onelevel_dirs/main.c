#include "header.h"

int main(void)
{
	int result;
	result = add(1, 2);
	printf("add=%d\n", result);
	result = sub(2, 1);
	printf("sub=%d\n", result);
	float r;
	r = mysqrt(16.0f);
	printf("sqrt=%.2f\n", r);
	return 0;

}
