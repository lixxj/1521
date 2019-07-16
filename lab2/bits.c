// COMP1521 19t2 ... lab 2 warm-up
// XJ

#include <stdio.h>
#include <stdlib.h>

static void range_uchar (void);
static void range_char (void);
static void range_uhint (void);
static void range_hint (void);
static void range_int (void);
static void range_uint (void);
static void range_lint (void);
static void range_ulint (void);
static void range_llint (void);
static void range_ullint (void);

int main (void)
{	
	printf("Min-Max calculations by XJ:\n\n");
	printf("              Data type  Size     Min  ...  Max\n");
	range_char ();
	range_uchar ();
	range_hint ();
	range_uhint ();
	range_int ();
	range_uint ();
	range_lint ();
	range_ulint ();
	range_llint ();
	range_ullint ();

	return EXIT_SUCCESS;
}

// Calculate and print the largest and smallest `int` values
static void range_int (void)
{
	int int_min = (-((unsigned int) ~0 >> 1) - 1);
	int int_max = ((unsigned int) ~0 >> 1);
	
	printf (
		"                   int (%zu bytes): %d ... %d\n",
		sizeof (int), int_min, int_max
	);
}

// Calculate and print the largest and smallest `unsigned int` values
static void range_uint (void)
{
	unsigned int uint_min = ((unsigned int) 0);
	unsigned int uint_max = ((unsigned int) ~0);
	printf (
		"          unsigned int (%zu bytes): %u ... %u\n",
		sizeof (unsigned int), uint_min, uint_max
	);
}

// Calculate and print the largest and smallest `long int` values
static void range_lint (void)
{
	long int long_min = (-((unsigned long int) ~0 >> 1) - 1);
	long int long_max = ((unsigned long int) ~0 >> 1);
	printf (
		"              long int (%zu bytes): %ld ... %ld\n",
		sizeof (long int), long_min, long_max
	);
}

// Calculate and print the largest and smallest `unsigned long int` values
static void range_ulint (void)
{
	unsigned long int ulong_min = (unsigned long int) 0;
	unsigned long int ulong_max = (unsigned long int) ~0;
	printf (
		"     unsigned long int (%zu bytes): %lu ... %lu\n",
		sizeof (unsigned long int), ulong_min, ulong_max
	);
}

// Calculate and print the largest and smallest `long long int` values
static void range_llint (void)
{
	long long int llong_min = (-((unsigned long long int) ~0 >> 1) - 1);
	long long int llong_max = ((unsigned long long int) ~0 >> 1);
	printf (
		"         long long int (%zu bytes): %lld ... %lld\n",
		sizeof (long long int), llong_min, llong_max
	);
}

// Calculate and print the largest and smallest `unsigned long long int` values
static void range_ullint (void)
{
	unsigned long long int ullong_min = (unsigned long long int) 0;
	unsigned long long int ullong_max = (unsigned long long int) ~0;
	printf (
		"unsigned long long int (%zu bytes): %llu ... %llu\n",
		sizeof (unsigned long long int), ullong_min, ullong_max
	);
}

// Calculate and print the largest and smallest `short int` values
static void range_hint (void)
{
	short int hint_min = (-((unsigned short int) ~0 >> 1) - 1);
	short int hint_max = ((unsigned short int) ~0 >> 1);
	printf (
		"             short int (%zu bytes): %hd ... %hd\n",
		sizeof (short int), hint_min, hint_max
	);
}

// Calculate and print the largest and smallest `unsigned short int` values
static void range_uhint (void)
{
	unsigned short int uhint_min = (unsigned short int) 0;
	unsigned short int uhint_max = (unsigned short int) ~0;
	printf (
		"    unsigned short int (%zu bytes): %hu ... %hu\n",
		sizeof (unsigned short int), uhint_min, uhint_max
	);
}

// Calculate and print the largest and smallest `char` values
static void range_char (void)
{
	char char_min = (-((unsigned char) ~0 >> 1) - 1);
	char char_max = ((unsigned char) ~0 >> 1);
	printf (
		"                  char (%zu bytes): %d ... %d\n",
		sizeof (char), char_min, char_max
	);
}

// Calculate and print the largest and smallest `unsigned char` values
static void range_uchar (void)
{
	unsigned char uchar_min = (unsigned char) 0;
	unsigned char uchar_max = (unsigned char) ~0;
	printf (
		"         unsigned char (%zu bytes): %u ... %u\n",
		sizeof (unsigned char), uchar_min, uchar_max
	);
}
