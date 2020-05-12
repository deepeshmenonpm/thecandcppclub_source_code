#include <stdio.h>


int my_variadic_function(const int arg_count, ...)
{
	const int *const argument_stack_address = &arg_count + 1;
	const int *arg_pointer = argument_stack_address;
	
	for(int i = 0; i < arg_count; ++i)
	{
		printf("arg %d: %d\n", i, arg_pointer[i]);
	}
	
	return 0;
}

int main()
{	
	my_variadic_function(5, 10, 20, 30, 40, 50);
	
	return 0;
}