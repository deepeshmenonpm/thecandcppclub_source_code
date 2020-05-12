#include <stdio.h>


void my_function(void)
{
	int i;
	char c;
	float f;
	
	printf("Address of i : 0x%X\n", &i);
	printf("Address of c : 0x%X\n", &c);
	printf("Address of f : 0x%X\n", &f);
	
	return;
}


int main()
{
	my_function();
	
	return 0;
}
