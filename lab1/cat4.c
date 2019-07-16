// Written by XJ
// the most efficient cat-alike version

#include <stdio.h>
#include <stdlib.h>

static void copy(FILE *, FILE *);

int main (int argc, char *argv[])
{
	if(argc == 1)
	{
		copy(stdin, stdout);
	} else
	{
		for(int i = 1; i < argc; i++) 
		{
			FILE *input = fopen(argv[i], "r");
			if (input == NULL)
			{
				perror("Can't read name-of-file");
			} else
			{
				copy(input, stdout);
				fclose(input);
			}
		}
	}

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

	char str[BUFSIZ];
	while(fgets(str, BUFSIZ, input) != NULL)
	{
		fputs(str, output);
   	}
}
