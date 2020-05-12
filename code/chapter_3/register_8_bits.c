#include <stdio.h>

typedef char byte;

#define REG8_MASK 0x000000FFU

typedef union register_8_bits__
{
	byte value;
	
	struct bits__
	{
		byte b0 : 1;
		byte b1 : 1;
		byte b2 : 1;
		byte b3 : 1;
		byte b4 : 1;
		byte b5 : 1;
		byte b6 : 1;
		byte b7 : 1;
	}bits;
	
	struct nibbles__
	{
		byte low : 4;
		byte high : 4;
	}nibbles;
	
}register_8_bits;




int main()
{
	register_8_bits my_reg;
	my_reg.value = 0x00; // clear register 
	
	printf("Register value : %X\n", my_reg.value & REG8_MASK);
	
	my_reg.bits.b2 = 1; // sets b2
	my_reg.bits.b7 = 1; // sets b7
	
	printf("Register value : %X\n", my_reg.value & REG8_MASK);
	printf("Register value low : %X\n", my_reg.nibbles.low &0xF);
	printf("Register value high : %X\n", my_reg.nibbles.high &0xF);
	
	my_reg.value = 0x00;
	
	// for setting bit 2
	my_reg.bits.b2 = 1;
	
	printf("b2 set, Register value : 0x%X\n", my_reg.value);

	// for clearing bit 2 
	my_reg.bits.b2 = 0;
	
	printf("b2 cleared, Register value : 0x%X\n", my_reg.value);
	
	// for setting b1
	my_reg.bits.b1 = 1;
	
	printf("b1 set, Register value : 0x%X\n", my_reg.value);

	// for setting high nibble bits to '1110'
	my_reg.nibbles.high = 0xE; // 0xE is 1110 in binary
	
	printf("nibble high is set to 0xE, Register value : 0x%X\n", my_reg.value & 0xFF);

	// for setting the entire 8 bits to 0xFF
	my_reg.value = 0xFF;
	
	printf("Value set to 0xFF, Register value : 0x%X\n", my_reg.value & 0xFF);
	
	return 0;
}
