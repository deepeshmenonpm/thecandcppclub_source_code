#include <stdio.h>

#ifdef DO_NOT_COMPILE

int function( int param0, int param1 )
{
	int var0;
	int var1;
	// do some computation here 
	return var0;
}

#endif

int functionB(int i)
{
	int j;
	
	// do something - compute j from i
	
	return j;
}

int functionA(int x, int y)
{
	int a;
	int b;
	
	// do some computation
	
	// calls functionB()
	b  = functionB(a);
	
	return b;
}


int main(int argc, char **argv)
{
	int return_status = 0;
	int temp1 = 0, temp2 = 0;
	
	return_status = functionA(temp1, temp2);
	
	return 0;
}

