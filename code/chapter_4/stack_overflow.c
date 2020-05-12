#include <stdio.h>

void dangerous_arrays(int arg1)
{
	int i = 1;
	int j = 2;
	int k = 3;
	int l = 4;
	
	char array[4] = {0, 0, 0, 0};
	
	printf("before stack corruption i : 0x%X\n", i);
	printf("before stack corruption j : 0x%X\n", j);
	printf("before stack corruption k : 0x%X\n", k);
	printf("before stack corruption l : 0x%X\n", l);
	
	printf("before stack corruption array[0] : 0x%X\n", 0xFF & array[0]);
	printf("before stack corruption array[1] : 0x%X\n", 0xFF & array[1]);
	printf("before stack corruption array[2] : 0x%X\n", 0xFF & array[2]);
	printf("before stack corruption array[3] : 0x%X\n", 0xFF & array[3]);
	
	for(int index = 0; index < 20; ++index)
	{
		array[index] = (char)(index * 10);
	}
	
	puts("---------------------------------------------------");
	
	printf("after stack corruption i : 0x%X\n", i);
	printf("after stack corruption j : 0x%X\n", j);
	printf("after stack corruption k : 0x%X\n", k);
	printf("after stack corruption l : 0x%X\n", l);
	
	puts("");
	
	for(int index = 0; index < 20; ++index)
	{
		printf("after stack corruption array[%d] : 0x%X\n", index, 0xFF & array[index]);
	}
	
	return;
}

int main()
{
	dangerous_arrays(0);
	
	return 0;
}

#ifdef DO_NOT_COMPILE

char array[10]; // hardcoded size for array is 10

// safer array use option 1; using sizeof operator
for(int index = 0; index < size(array)/sizeof(char); ++index)
{
	array[index] = index * 10;
}

// safer array use option 2; using a constant size information 
const int size = 4;
char array[size];

for(int index = 0; index < size; ++index)
{
	array[index] = index * 10;
}

// safer array use option 3; using a macro for size information
#define SIZE 4
char array[SIZE];

for(int index = 0; index < SIZE; ++index)
{
	array[index] = index * 10;
}

//If passing arrays into functions, follow the convention I have shown below:

// the parameter declaration 'int array[ ]' explicitly conveys that its as array 
void do_some_operation(int array[], const int size) 
{
	for(int index = 0; index < size; ++index)
	{
		// do something using array here
	}
	
	return;
}

// I dont recommend the following at all.
void do_some_operation(int *array){...}
void do_some_operation(int *array, const int size){...}

int function(int param0, int param1)
{
	int var0; 
	int var1; 
	// do some computation here 
	return var0; 
} 

#endif
 

