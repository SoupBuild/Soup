#include <stdio.h>

int main()
{
#ifdef SPECIAL_BUILD
	printf("Hello World, Soup Style!");
#else
	printf("Hello World...");
#endif
	return 0;
}
