#include <stdio.h>

typedef union 
{
	int i;
	char c;
	float f;
}my_union;

my_union u1;

#ifdef DO_NOT_COMPILE

u1.i = 5; // from here onwards u1 behaves as an integer till line 14
...

u1.f = 2.57F; // from here onwards u1 behaves as a float till line 17
...

u1.c = 'a'; // from here onwards u1 behaves as a character and so on...
...
#endif

int main()
{
my_union u1;
char *byte;
int b_count;

// use as integer
puts("As integer");
u1.i = 5;

byte = (char*)&(u1.i);
for(b_count = 0; b_count < sizeof(my_union); ++b_count)
{
	printf("Byte %d : 0x%X\n", b_count, byte[b_count]);
}

// use as float
puts("As float");
u1.f = 5.125F;

byte = (char*)&(u1.f);
for(b_count = 0; b_count < sizeof(my_union); ++b_count)
{
	printf("Byte %d : 0x%X\n", b_count, (byte[b_count]&0xFF));
}

// use as char
puts("As char");
u1.c = 'A';

byte = (char*)&(u1.c);
for(b_count = 0; b_count < sizeof(my_union); ++b_count)
{
	printf("Byte %d : 0x%X\n", b_count, (byte[b_count]&0xFF));
}

puts("\nWhere is the union 'u1' stored ?\n");
printf("Address of u1.i is 0x%X\n", &u1.i);
printf("Address of u1.f is 0x%X\n", &u1.f);
printf("Address of u1.c is 0x%X\n", &u1.c);

}

#ifdef DO_NOT_COMPILE

register_8_bits my_reg;
my_reg.value = 0x00; // clear register 

printf("Register value : %X\n", my_reg.value & REG8_MASK);

my_reg.bits.b2 = 1; // sets b2
my_reg.bits.b7 = 1; // sets b7

printf("Register value : %X\n", my_reg.value & REG8_MASK);
printf("Register value low : %X\n", my_reg.nibbles.low &0xF);
printf("Register value high : %X\n", my_reg.nibbles.high &0xF);

#endif
