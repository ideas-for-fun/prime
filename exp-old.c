#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/malloc.h>
#include <math.h>

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
} str2int_errno;

// 256 bit arithmetic

typedef unsigned long long u64b;
typedef __uint128_t u128b;
typedef struct u256b u256b;
struct u256b
{
	u64b lo;
	u64b mid;
	u128b hi;
};

int powers[128];
int coefficients[128];

u256b add256b(u256b *x, u256b *y)
{
	u128b lo = (u128b) x->lo + y->lo;
	u128b mid = (u128b) x->mid + y->mid + (lo >> 64);
	u256b result =
	{
		.lo = lo,
		.mid = mid,
		.hi = x->hi + y->hi + (mid >> 64),
	};
	
	return result;
}

/* Dumb n^2 way */
u256b mul256b(u256b *x, u256b *y)
{
	u128b t1 = (u128b) x->lo * y->lo;
	u128b t2 = (u128b) x->lo * y->mid;
	u128b t3 = x->lo * y->hi;
	u128b t4 = (u128b) x->mid * y->lo;
	u128b t5 = (u128b) x->mid * y->mid;
	u64b t6 = x->mid * y->hi;
	u128b t7 = x->hi * y->lo;
	u64b t8 = x->hi * y->mid;

	u64b lo = t1;
	u128b m1 = (t1 >> 64) + (u64b)t2;
	u64b m2 = m1;
	u128b mid = (u128b) m2 + (u64b)t4;
	u128b hi = (t2 >> 64) + t3 + (t4 >> 64) + t5 + ((u128b) t6 << 64) + t7
		 + ((u128b) t8 << 64) + (m1 >> 64) + (mid >> 64);
	
	u256b result = {lo, mid, hi};
	
	return result;
}






str2int_errno str2int(__uint128_t *out, char *s, int base) {
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    __uint128_t l = strtoll(s, &end, base);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
/*    
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
*/
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
}

int print_uint128(__uint128_t n) {
  if (n == 0)  return printf("0/n");

  char str[40] = {0}; // log10(1 << 128) + '/0'
  char *s = str + sizeof(str) - 1; // start at the end
  while (n != 0) {
    if (s == str) return -1; // never happens

    *--s = "0123456789"[n % 10]; // save last digit
    n /= 10;                     // drop it
  }
  return printf("%s\n\n", s);
}

int convert_64bit_to_polynomial(unsigned long long num)
{
	unsigned long long mask = 0L;
	unsigned long long product = 0L, productcomp = 0L;
	mask = 0x8000000000000000;

	int i,j;

	for (i = 0 ; i < 128 ; i++)
	{
		powers[i] = 0;
		coefficients[i] = 0;
	}

	for (i = 0 ; i < 64 ; i++)
	{

		if (num & mask)
		{
			powers[i] = 1;
			coefficients[i] = 1;
		}
		printf("%d iteration powers[%d]=%d coefficients[%d]=%d \n", i, i, powers[i], i, coefficients[i]);
		mask = mask >> 1;
	}

	for (i = 63 , j = 0 ; i >= 0; i--, j++)
	{
		if (coefficients[i] == 1)
		{
			printf("%d->%d,", i, coefficients[i]);
			productcomp = (unsigned long long) pow((double) 2, (double) j);
			product += productcomp;
		}
	}
	printf("\n\n%llu is the product after polynomial summation\n\n", product);
	return 0;
}

unsigned long long c1terms[128];
unsigned long long c2terms[128];
int c1terms_powers[128];
int c2terms_powers[128];
int c1terms_compdiv[128];
int c2terms_compdiv[128];
int consec_c1terms[128];
int consec_c2terms[128];
int summation_polynomial[128];
unsigned long long error = 0;
__uint128_t c1compute = 0;
__uint128_t c2compute = 0;

void build_c1_and_c2(__uint128_t product)
{

// Initialize c1terms and c2terms

	int i = 0, j = 0, k = 0;
	int termcount = 0;
	int divparts = 0;
	int c1termcount = 0;
	int c2termcount = 0;
	int countc1 = 0, countc2 = 0, count = 0;
	int ii = 0, kk = 0;
	int polycount = 0;

	for (i = 0 ; i < 64 ; i++)
	{
		c1terms[i] = 0L;
		c2terms[i] = 0L;
		c1terms_powers[i] = 0; // Contains power div 2 terms
		c2terms_powers[i] = 0; // Contains power div 2 terms
		c1terms_compdiv[i] = 0;// Divisor of final c1term
		c2terms_compdiv[i] = 0;// Divisor of final c2term
		summation_polynomial[i] = 0;
		summation_polynomial[64+i] = 0;
	}

// Lookup powers and count the terms in P(x)
	termcount = 0;

	for (i = 0 ; i < 64 ; i++)
	{
		if (powers[i] == 1)
		{
			termcount++;
		}
	}

// Divide termcount / 2.
// See if sum of divided parts comes to termcount again.

	divparts = termcount / 2;
	if (( 2 * divparts ) != termcount)
	{
		if ((2 * divparts) > termcount)
		{
			c1termcount = divparts - 1 ;
			c2termcount = divparts;
		}
		else
		{
			c1termcount = divparts ;
			c2termcount = divparts + 1;
		}
	}
// Compute c1terms_powers and c2terms_powers in i in place 
// Compute also the c1, c2 compdiv terms as well in i in place.

// Compute c2terms first Populated from 63 to 33 in i
	count = c2termcount ;

	for (i = 63 , j = 0, k = 0; count ; i--, j++, k++)
	{
		if (powers[i] == 1)
		{
			c2terms_powers[i] =  j;
			consec_c2terms[k] =  j;
			count--;
		}
	}

// Compute c1terms second Populated from 32 to 0 in i 
	count = c1termcount ;

	for (i = 32 , k = 0; count ; i--, j++, k++)
	{
		if (powers[i] == 1)
		{
			c1terms_powers[i] = j / 2;
			consec_c1terms[k] = j / 2;
			count--;
		}
	}

// So 63 will be LSBit and 0 will be the MSBit
// Compute MULTIPLICATION of C1 with C2 to get intermediate result

	count = c2termcount;
	polycount = 0;

	for (i = 63 ; count ; i--)
	{
		if (powers[i] == 1)
		{
			for (k = 32 ; k >= 0 ; k--) 
			{

				if (powers[k] == 1)
				{
					summation_polynomial[c2terms_powers[i] + c1terms_powers[k]] = 1;
					polycount++;
				}
			}

			count--;
		}
	}

// TODO : There is a problem in this part of the code .... FIX IT.
// It relates to computing the compdiv and the final C1, C2 compute.

// Compute the c1terms_compdiv and c2terms_compdiv

	count = polycount;
	countc1 = c1termcount;
	countc2 = c2termcount;
	c2compute = 0;
	c1compute = 0;
	for ( i = 128 , ii = 0, k = 63, kk = 0; count ; i--, ii++, kk++)
	{
		if (summation_polynomial[i] == 1) 
		{
			while (powers[k] == 0)
			{
				k--; kk++;
			}
			// original polynomial in powers
			// Pprime(x) in summation_polynomial
			if (countc2)
			{
				if (i > k)
					c2terms_compdiv[k] = ii - kk;
				else if (i == k)
					c2terms_compdiv[k] = 1;
				else
				{
					// Fix this later... requires summation_polynomial boost up
				}
				c2compute += ((__uint128_t) pow((double) 2, (double) ii)) / 
						((__uint128_t) pow((double) 2, (double) c2terms_compdiv[k]));
				countc2--;
			}
			else if (countc1)
			{
				if (i > k)
					c1terms_compdiv[k] = ii - kk;
				else if (i == k)
					c1terms_compdiv[k] = 1;
				else
				{
					// Fix this later... requires summation_polynomial boost up
				}
				c1compute += ((__uint128_t) pow((double) 2, (double) ii)) / 
						((__uint128_t) pow((double) 2, (double) c1terms_compdiv[k]));
				countc1--;
			}
			count--;
		}
	}

// Print c1compute and c2compute
	print_uint128(c1compute);
	print_uint128(c2compute);
			

}


int main(int argc, char *argv[])
{
//	printf ("Hello world \n");

	unsigned long long i;
	__uint128_t j;
//	__uint256_t k;
	unsigned long long p2 = 217644967L;
	unsigned long long p1 = 217644859L; 

	j = (__uint128_t) p1 * p2 ;

	printf ("P1 and P2 product is : ");
	print_uint128(j);

	unsigned long long p3 = 982451501L; 
	unsigned long long p4 = 982451549L;

	j = (__uint128_t) p3 * p4 ;

	printf ("P3 and P4 product is : ");
	print_uint128(j);
	
	long double p5 ;
	// printf("%d is result of p5 conversion\n", str2int(&p5, "6201762057622736230362323", 10));
	p5 = (long double) 6201762057622736230362323.0;
	long double p6 ; 
	// printf("%d is result of p6 conversion\n", str2int(&p6, "9989999899883889989999899", 10));
	p6 = (long double) 9989999899883889989999899.0;

	j = (__uint128_t) p5 * p6 ;

	printf ("P5 and P6 product is : ");
	print_uint128(j);

	long double po = 2;

	printf("%30f power of 2 raised to 128\n",pow((double) po,(double) 128));

	convert_64bit_to_polynomial(p3);
	convert_64bit_to_polynomial(p4);
	j = (__uint128_t) p3 * p4 ;
	build_c1_and_c2(j);
	// c1 and c2 are in c1terms and c2terms
//	break_product(j, c1terms, c2terms, error);

//	printf("Size of unsigned long long %ld \n",sizeof(i));
//	printf("Size of unsigned long long %ld \n",sizeof(j));
//	printf("Size of unsigned long long %ld \n",sizeof(long double));
}
