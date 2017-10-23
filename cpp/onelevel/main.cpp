#include <iostream>
#include "mycode1.hpp"
#include "mycode2.h"

using namespace std;

int main(void)
{
	Calculator1 cc1;
	Calculator2 cc2;

	cout << cc1.add(1, 2) << endl;
	cout << cc2.add(3, 4) << endl;
	return 0;
}
