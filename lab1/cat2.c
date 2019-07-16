// Written by XJ
// cat2: Copy input to output using fgetc(2) and fputc(3)

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

	int c; // int fgetc(FILE *pointer)
	while((c = fgetc(input)) != EOF) 
	{ 
        fputc(c, output);      
    }
}
