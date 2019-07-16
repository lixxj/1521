// Written by XJ
// cat1: Copy input to output using fscanf(3) and fprintf(3)

#include <stdio.h>
#include <stdlib.h>

static void copy(FILE *, FILE *);

int main(int argc, char *argv[])
{
	copy(stdin, stdout);
	return EXIT_SUCCESS;
}

// Copy contents of input to output, char-by-char
// Assumes both files open in appropriate mode
static void copy (FILE *input, FILE *output)
{
	/*if(input == NULL || output == NULL)
	{
		perror("IOError");
		return;
   	}*/

	char c; 
	while(fscanf(input, "%c", &c) == 1) 
	{
		fprintf(output, "%c", c);
	}
}
