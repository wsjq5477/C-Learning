#include<iostream>
#include<cstdio>
using namespace std;

int main()
{
	char ulTemp[100] = {0};

	while (gets_s(ulTemp, 100) != NULL)
	{
		printf("ÊäÈëÖµÎª£º%s\n", ulTemp);
	}
	return 0;
}