// BigNum.c ... LARGE positive(and negative by XJ) integer values
// Modified by XJ

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BigNum.h"

#define TRUE      1
#define FALSE     0
#define NEGATIVE  'n'
#define POSITIVE  'p'

// int <=> char
#define ctoi(c) (c - '0')
#define itoc(i) (i + '0')

// Initialise a BigNum to N bytes, ALL '0', POSITIVE
static void initZero (BigNum *bn)
{
	for(int i = 0; i < bn->nbytes; i++) 
	{
		bn->bytes[i] = '0';
	}
	bn->bytes[bn->nbytes] = POSITIVE;
}

// Initialise a BigNum to N bytes, all '0'
void initBigNum (BigNum *bn, int Nbytes)
{ 
	bn->nbytes = Nbytes;// setting the byte counter
	bn->bytes = malloc((bn->nbytes + 1) * sizeof(Byte)); // create an array of length Nbytes + sign byte
	initZero(bn);
	assert(bn->bytes != NULL); // use assert to check whether the array was successfully created
	return; // no return value
}

// Display a BigNum in decimal format
void showBigNum (BigNum bn)
{
	// sign part
	if(bn.bytes[bn.nbytes] == NEGATIVE) // negative numbers
	{	
		printf("-");
	} 
	// numerical part
	int padding_zero_flag = TRUE; // flag to handle padding zeros
	// !!! Note padding zero should not exist: controlled by scanning process !!!
	for(int i = bn.nbytes - 1; i >= 0; i--) 
	{
		if(bn.bytes[i] == '0' && padding_zero_flag == TRUE) // padding zeros
		{
			continue;
		}
		padding_zero_flag = FALSE; // numerical part starts
		printf("%c", bn.bytes[i]);
	}
	if(padding_zero_flag == TRUE) // no non-'0' digit
	{
		printf("0");
	}
	
	return;
}

// Set the value of a BigNum from a string of digits
// Returns 1 if it *was* a string of digits, 0 otherwise
int scanBigNum (char *s, BigNum *bn)
{
	int valid_num = FALSE;
	int slen = strlen(s);
	int start_reading_flag = FALSE; // should called start_reading_digits_flag
	int sign_flag = FALSE;
	int start_loc = 0;
	int end_loc = slen - 1;
	// scan 
	for(int i = 0; i < slen; i++)
	{
		if(start_reading_flag == FALSE && isspace(s[i])) // leading space characters
		{
			continue;
		}
		if(start_reading_flag == FALSE && s[i] == '0') // leading '0' characters
		{
			continue;
		}
		if(start_reading_flag == FALSE && sign_flag == FALSE && s[i] == '-') // '-' sign character
		{
			sign_flag = TRUE;
			continue;
		}
		if(start_reading_flag == FALSE && !isdigit(s[i])) // first non-space is non-digit or multiple sign error
		{
			valid_num = FALSE;
			break;
		}
		if(start_reading_flag == FALSE && isdigit(s[i])) // first non-space is digit
		{
			start_reading_flag = TRUE;
			valid_num = TRUE;
			start_loc = i;
			continue;
		}
		if(start_reading_flag == TRUE && isdigit(s[i]) == FALSE) // read until non-digit or end of string
		{
			end_loc = i - 1;
			break;
		}
	}
	// store
	if (valid_num == TRUE) 
	{
		// resize to BigNum size + sign byte
		bn->nbytes = (end_loc - start_loc + 1);
		bn->bytes = realloc(bn->bytes, ((bn->nbytes) + 1));
		initZero(bn);
		if(s[start_loc] > 0 && s[start_loc - 1] == '-') // store sign
		{
			bn->bytes[bn->nbytes] = NEGATIVE;
		} else 
		{
			bn->bytes[bn->nbytes] = POSITIVE;
		}
		// store digits
		int i_byte = 0; // index of bytes in bn
		for(int j = end_loc; j >= start_loc; j--) 
		{
			bn->bytes[i_byte] = s[j];
			i_byte++;
		}
	} else // valid_num == FALSE
	{
		bn->nbytes = (1); // '0'
		bn->bytes = realloc(bn->bytes, ((bn->nbytes) + 1));
		initZero(bn);
	}
	
	return valid_num;
}

// Return the BigNum with smaller absolute value(BigNums have same length)
static BigNum same_length_smallerabs (BigNum bnA, BigNum bnB)
{
	BigNum smaller = bnA;
	for(int i = (bnA.nbytes - 1); i >= 0; i--) 
	{
		if (bnA.bytes[i] > bnB.bytes[i])
		{
			smaller = bnB;
		} 
		if (bnA.bytes[i] < bnB.bytes[i])
		{
			smaller = bnA;
		}
		if (bnA.bytes[i] == bnB.bytes[i])
		{
			continue;
		}
	}

	return smaller;
}

// Return the BigNum with smaller absolute value
static BigNum smallerabs (BigNum bnA, BigNum bnB)
{
	BigNum smaller;
	if(bnA.nbytes > bnB.nbytes)
	{
		smaller = bnB;
	} else if(bnA.nbytes < bnB.nbytes)
	{
		smaller = bnA;
	} else // bnA.nbytes == bnB.nbytes
	{
		smaller = same_length_smallerabs (bnA, bnB); // byte by byte check		
	}
	
	return smaller;
}

// Return the BigNum with larger absolute value(BigNums have same length)
static BigNum same_length_largerabs (BigNum bnA, BigNum bnB)
{
	BigNum larger = bnB;
	for(int i = (bnA.nbytes - 1); i >= 0; i--) 
	{
		if (bnA.bytes[i] < bnB.bytes[i])
		{
			larger = bnB;
		} 
		if (bnA.bytes[i] > bnB.bytes[i])
		{
			larger = bnA;
		}
		if (bnA.bytes[i] == bnB.bytes[i])
		{
			continue;
		}
	}

	return larger;
}

// Return the BigNum with larger absolute value
static BigNum largerabs (BigNum bnA, BigNum bnB)
{
	BigNum larger;
	if(bnA.nbytes > bnB.nbytes)
	{
		larger = bnA;
	} else if(bnA.nbytes < bnB.nbytes)
	{
		larger = bnB;
	} else // bnA.nbytes == bnB.nbytes
	{
		larger = same_length_largerabs (bnA, bnB); // byte by byte check 
	}

	return larger;
}

// Add two BigNum absolute values and store result in a third BigNum
static void absaddBigNums (BigNum bnS, BigNum bnL, BigNum *res) 
{
	int carry = 0;
	int sum = 0; // intermediate sum
	for(int i1 = 0; i1 < bnS.nbytes; i1++) // lower digits
	{
		sum = ctoi(bnS.bytes[i1]) + ctoi(bnL.bytes[i1]) + carry;
		res->bytes[i1] = itoc(sum % 10);
		carry = sum / 10;
	}
	int i2; 
	for(i2 = bnS.nbytes; i2 < bnL.nbytes; i2++) // higher digits
	{
		sum = ctoi(bnL.bytes[i2]) + carry;
		res->bytes[i2] = itoc(sum % 10);
		carry = sum / 10;
	}
	if(carry > 0) {
		res->bytes[i2] = itoc(carry);
	}

	return;
}

// Add two BigNums and store result in a third BigNum
void addBigNums (BigNum bnA, BigNum bnB, BigNum *res)
{
	BigNum bnS = smallerabs (bnA, bnB);
	BigNum bnL = largerabs (bnA, bnB);
	res->nbytes = bnL.nbytes + 1; // larger* BigNum + (potential)carry
	res->bytes = realloc(res->bytes, ((res->nbytes) + 1)); // resize to BigNum size + sign byte
	initZero(res);
	// check, change sign and call computation functions
	if (bnS.bytes[bnS.nbytes] == POSITIVE && bnL.bytes[bnL.nbytes] == POSITIVE) // P + P
	{
		res->bytes[res->nbytes] = POSITIVE;
		absaddBigNums (bnS, bnL, res);
	} else if (bnS.bytes[bnS.nbytes] == NEGATIVE && bnL.bytes[bnL.nbytes] == NEGATIVE) // N, N => - (P + P)
	{
		res->bytes[res->nbytes] = NEGATIVE;
		absaddBigNums (bnS, bnL, res);
	} else if (bnA.bytes[bnA.nbytes] == POSITIVE && bnB.bytes[bnB.nbytes] == NEGATIVE) // P + N
	{
		// if abs(bnA) < abs(bnB)
			// res->sign->negative
		// abssubtractBigNums (bnS, bnL, res);
	} else if (bnA.bytes[bnA.nbytes] == NEGATIVE && bnB.bytes[bnB.nbytes] == POSITIVE) // N + P
	{
		// if abs(bnA) > abs(bnB)
			// res->sign->negative
		// abssubtractBigNums (bnS, bnL, res);
	} 

	return;
}
