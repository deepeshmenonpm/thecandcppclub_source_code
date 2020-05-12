#include <stdio.h>

typedef struct
{
	int i;
	int j;
	int k;
}my_struct;

my_struct func1(void)
{
	my_struct s = {1,2,3};
	return s;
}

int main()
{
	my_struct s0 = func1();	// C will not support return value of more than 4 bytes directly.
							// Here, C compiler changes the above function call to 
							// func1(&s0); For the programmer, even if func1() is a no argument function,
							// compiler makes func1() to a single argument function
							// Do you remember in C++ all non-static member functions 
							// get this* (a.k.a this pointer) as their first parameter 
							// from the compiler automatically ?
	
	printf("so.i : %d\n", s0.i);
	printf("so.j : %d\n", s0.j);
	printf("so.k : %d\n", s0.k);
	
	return 0;
}