#include <stdio.h>

typedef struct __attribute__((packed))
{
	int i1;
	int i2;
	double d1;
	char c1;
}my_struct_t;

my_struct_t s1;

int main()
{
	
	size_t size1 = sizeof(my_struct_t);
	
	unsigned int address_of_s1 = (unsigned int)&(s1);
	unsigned char *byte = (unsigned char *)address_of_s1;
	
	unsigned int address_of_i1 = (unsigned int)&(s1.i1);
	unsigned int address_of_i2 = (unsigned int)&(s1.i2);
	
	unsigned int address_of_d1 = (unsigned int)&(s1.d1);
	
	unsigned int address_of_c1 = (unsigned int)&(s1.c1);
	
	
	s1.i1 = 0x12345678;
	s1.i2 = 0x9ABCDEF0;
	*((unsigned long long*)address_of_d1) = 0xABABABABABABABABU;
	s1.c1 = 0xCC;
	
	printf("Size of struct : %U\n", size1);
	printf("address of s1 : 0x%X\n", address_of_s1);
	
	printf("address_of_i1 : 0x%X\n", address_of_i1);
	printf("address_of_i2 : 0x%X\n", address_of_i2);
	
	printf("address_of_d1 : 0x%X\n", address_of_d1);

	printf("address_of_c1 : 0x%X\n", address_of_c1);
	
	for(int i = 0; i < size1; ++i)
	{
		printf("byte[%d] : 0x%X\n", i, 0xFF & byte[i]);
	}
	
	return 0;
}