// Read in bit strings to construct float values using bit-fields and unions
// Written by John Shepherd, August 2017
// Completed by XJ

#include <assert.h>
#include <err.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

typedef uint32_t word;

typedef struct float32 { // bit_fields for sign, exp and frac
	unsigned int frac : 23;
	unsigned int exp : 8;
	unsigned int sign : 1; 
} float32;

typedef union bits32 {
	float fval;   // interpret the bits as a float
	word xval;    // interpret as a single 32-bit word
	float32 bits; // manipulate individual bits
} bits32;

void checkArgs (int, char **);
bits32 getBits (char *, char *, char *);
char *showBits (word, char *);
bool justBits (char *, int);
static unsigned int bstou (char *str); // bit-string to unsigned int

int main (int argc, char **argv)
{
	bits32 u;
	char out[50];

	u.bits.sign = u.bits.exp = u.bits.frac = 0;

	// check command-line args (all strings of 0/1
	// kills program if args are bad
	checkArgs (argc, argv);

	// convert command-line args into components of
	// a float32 inside a bits32, and return the union
	u = getBits (argv[1], argv[2], argv[3]);

	printf ("bits : %s\n", showBits (u.xval, out));
	printf ("float: %0.10f\n", u.fval);

	return EXIT_SUCCESS;
}

// convert three bit-strings (already checked)
// into the components of a struct _float
bits32 getBits (char *sign, char *exp, char *frac)
{
	bits32 new;
	new.bits.sign = bstou(sign);
	new.bits.exp = bstou(exp);
	new.bits.frac = bstou(frac);

	return new;
}

static unsigned int bstou(char *str) // bit-string to unsigned int
{
	int res = 0;
	for (int i = (strlen(str) - 1), j = 0; i >= 0; i--, j++)
	{
		if (str[i] == '1')
		{
			res = res + (1 << j);
		}
	}

	return res;
}

// convert the 32-bit bit-string in val into
// a sequence of '0' and '1' characters in buf
// assume that buf has size > 32
// return a pointer to buf
char *showBits (word val, char *buf)
{
	int j = 31; // bit counter
	int i; // buf counter
	for (i = 0; i < 34; i++) 
	{
		if (i == 1 || i == 10) // space between components
		{
			buf[i] = ' ';
			continue;
		}
		if ((val & (1 << j)) == (1 << j)) // bit is set
		{
			buf[i] = '1';
		}
		if ((val & (1 << j)) == 0) // bit is clear
		{
			buf[i] = '0';
		}
		j--;
	}
	buf[i] = '\0'; // This is required on CSE Linux, but not necessary on MacOs. XJ
	
	return buf;
}

// checks command-line args
// need at least 3, and all must be strings of 0/1
// never returns if it finds a problem
void checkArgs (int argc, char **argv)
{
	if (argc < 3)
		errx (EX_USAGE, "usage: %s Sign Exp Frac", argv[0]);

	if (! justBits (argv[1], 1))
		errx (EX_DATAERR, "invalid Sign: %s", argv[1]);
	if (! justBits (argv[2], 8))
		errx (EX_DATAERR, "invalid Exp: %s",  argv[2]);
	if (! justBits (argv[3], 23))
		errx (EX_DATAERR, "invalid Frac: %s", argv[3]);

	return;
}

// check whether a string is all 0/1 and of a given length
bool justBits (char *str, int len)
{
	assert (len >= 0);
	if (strlen (str) != (size_t) len)
		return false;

	while (*str != '\0') {
		if (*str != '0' && *str != '1')
			return false;
		str++;
	}
	return true;
}
