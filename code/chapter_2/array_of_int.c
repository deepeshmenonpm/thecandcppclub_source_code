#include <stdio.h>


int main()
{
int arr_of_int[5] = {1, 2, 3, 4, 5};

printf("arr_of_int[0] : %d\n", arr_of_int[0]);
printf("arr_of_int[1] : %d\n", arr_of_int[1]);
printf("arr_of_int[2] : %d\n", arr_of_int[2]);
printf("arr_of_int[3] : %d\n", arr_of_int[3]);
printf("arr_of_int[4] : %d\n", arr_of_int[4]);

puts("--------------------------------------");

printf("0[arr_of_int] : %d\n", 0[arr_of_int]);
printf("1[arr_of_int] : %d\n", 1[arr_of_int]);
printf("2[arr_of_int] : %d\n", 2[arr_of_int]);
printf("3[arr_of_int] : %d\n", 3[arr_of_int]);
printf("4[arr_of_int] : %d\n", 4[arr_of_int]);

puts("--------------------------------------");

int array[3] = {1, 2, 3};
int *pointer = &(array[1]);

printf("*pointer : %d\n", *pointer);
printf("pointer[0] : %d\n", pointer[0]);

return 0;
}