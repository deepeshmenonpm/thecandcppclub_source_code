#include <stdio.h>

typedef void function_type(int a);
typedef void (*function_ptr_type)(int a);
void (*function_ptr)(int a); // not a type definition, 

void function1(int i)
{
	printf("i : %d\n", i);
}

int main()
{
	function_type *f_ptr_1 = function1; // &function1 is not required, 
										// as function name, 
										// just like array names gives 
										// function's address
	f_ptr_1(1);
	
	function_ptr_type f_ptr_2 = function1;
	(*f_ptr_2)(2); 						// or simply, f_ptr(2). No need 
										// to explicitly dereference. 
	
	function_ptr = function1;			// Here function_ptr is actually 
										// a function pointer variable 
	function_ptr(3);	
	
	return 0;
}

