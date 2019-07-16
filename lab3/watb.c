// Reveal internal layout of bit-fields
// Completed by XJ

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
	unsigned int a : 4;
	unsigned int b : 8;
	unsigned int c : 20;
};

int main (void)
{
	struct _bit_fields x = {0, 0, 0}; // construction of bit-field
    
   	unsigned int *ptrX = (unsigned int *) &x; // bit-field as a single unit(unsigned int)
	printf("%u\n", *ptrX);

	return EXIT_SUCCESS;
}

/*
layout1
	aaaabbbbbbbbcccccccccccccccccccc
	<-low        address      high->

layout2
	ccccccccccccccccccccbbbbbbbbaaaa
	<-low        address      high->	

experiment1
	set value of x.a to 1: 
	under layout1, 268435456(2^28) is expected;
	under layout2, 1 is expected.

experiment2
	set value of x.b to 1: 
	under layout1, 1048576(2^20) is expected;
	under layout2, 16(2^4) is expected.

experiment3
	set value of x.c to 1: 
	under layout1, 1 is expected;
	under layout2, 4096(2^12) is expected.

Any of the three experiments can be used to determine the layout being used. 
XJ
*/
