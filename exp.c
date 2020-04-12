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

static int powers[128];
static int coefficients[128];
static int diffpowers[128];
static int diffcoefficients[128];

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

int diffconvert_128bit_to_polynomial(__uint128_t num)
{
	__uint128_t mask = 0L;
	__uint128_t product = 0L, productcomp = 0L;
	int i,j;
	mask = 0x1;

	for (i = 0; i < 127; i++)
	{
		mask = mask << 1;
	}
		
//	mask = 0x80000000000000000000000000000000;


	for (i = 0 ; i < 128 ; i++)
	{
		diffpowers[i] = 0;
		diffcoefficients[i] = 0;
	}

	for (i = 0 ; i < 128 ; i++)
	{

		if (num & mask)
		{
			diffpowers[i] = 1;
			diffcoefficients[i] = 1;
		}
		printf("%d iteration diffpowers[%d]=%d diffcoefficients[%d]=%d \n", i, i, diffpowers[i], i, diffcoefficients[i]);
		mask = mask >> 1;
	}

	for (i = 127 , j = 0 ; i >= 0; i--, j++)
	{
		if (diffcoefficients[i] == 1)
		{
			printf("%d->%d \n", j, diffcoefficients[i]);
			productcomp = (unsigned long long) pow((double) 2, (double) j);
			product += productcomp;
		}
	}
	printf("\n\nThis is the product after polynomial summation :");
	print_uint128(product);
	return 0;
}

int convert_128bit_to_polynomial(__uint128_t num)
{
	__uint128_t mask = 0L;
	__uint128_t product = 0L, productcomp = 0L;
	int i,j;
	mask = 0x1;

	for (i = 0; i < 127; i++)
	{
		mask = mask << 1;
	}
		
//	mask = 0x80000000000000000000000000000000;


	for (i = 0 ; i < 128 ; i++)
	{
		powers[i] = 0;
		coefficients[i] = 0;
	}

	for (i = 0 ; i < 128 ; i++)
	{

		if (num & mask)
		{
			powers[i] = 1;
			coefficients[i] = 1;
		}
		printf("%d iteration powers[%d]=%d coefficients[%d]=%d \n", i, i, powers[i], i, coefficients[i]);
		mask = mask >> 1;
	}

	for (i = 127 , j = 0 ; i >= 0; i--, j++)
	{
		if (coefficients[i] == 1)
		{
			printf("%d->%d \n", j, coefficients[i]);
			productcomp = (unsigned long long) pow((double) 2, (double) j);
			product += productcomp;
		}
	}
	printf("\n\nThis is the product after polynomial summation :");
	print_uint128(product);
	return 0;
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
static int terms_compdiv[1024];
int consec_c1terms[128];
int consec_c2terms[128];
static int summation_polynomial[1024];
unsigned long long error = 0;
__uint128_t c1compute = 0;
__uint128_t c2compute = 0;

#include "./exp-comp4.c"


int main(int argc, char *argv[])
{
//	printf ("Hello world \n");

	unsigned long long i;
	__uint128_t j, p13, p14;
//	__uint256_t k;
	unsigned long long p2 = 217644967L;
	unsigned long long p1 = 217644859L; 

	j = (__uint128_t) p1 * p2 ;

	printf ("P1 and P2 product is : ");
//	print_uint128(j);

	unsigned long long p3 = 982451501L; 
	unsigned long long p4 = 982451549L;

	j = (__uint128_t) p3 * p4 ;

	printf ("P3 and P4 product is : ");
//	print_uint128(j);
	
	long double p5 ;
	// printf("%d is result of p5 conversion\n", str2int(&p5, "6201762057622736230362323", 10));
	p5 = (long double) 6201762057622736230362323.0;
	long double p6 ; 
	// printf("%d is result of p6 conversion\n", str2int(&p6, "9989999899883889989999899", 10));
	p6 = (long double) 9989999899883889989999899.0;

	j = (__uint128_t) p5 * p6 ;

	printf ("P5 and P6 product is : ");
//	print_uint128(j);

	long double po = 2;

	printf("%30f power of 2 raised to 128\n",pow((double) po,(double) 128));

//	unsigned long long p3 = 982451501L; 
//	unsigned long long p4 = 982451549L;

#ifdef FIRST_TEST
	convert_64bit_to_polynomial(p3);
	convert_64bit_to_polynomial(p4);
	j = (__uint128_t) p3 * p4 ;
	build_c1_and_c2(j, p3);
	// c1 and c2 are in c1terms and c2terms
#endif

//	p13 = 597325496783;  // Original larger
//	p14 = 689101181569; // Original larger
	p13 = 3531577135439; 
	p14 = 5298696301807;
//	p13 = 9203225223029; // Original larger
//	p14 = 7142857142857; // Original larger
//	p13 = 485398038695407; // Original larger
//	p14 = 999998727899999; // Original larger
	p13 = 6082394749206781697; // Original larger
	p14 = 2220819602560918840; // Original larger

//	p13 = 7;
//	p14 = 11;
//	p13 = 982451501; 
//	p14 = 982451549;

	j = (__uint128_t) p13 * p14;
	build_c1_and_c2(j,p13);

// TODO : Final break right here. CODE IT.


#ifdef PRIMESONLY

123511311277 prime
124567987631 prime
125411328001 prime
126704222713 prime
127397154761 prime
129866728583 prime
130147795189 prime
131681894401 prime
131707310437 prime
136363636361 prime
137168442221 prime
137438691329 prime
137438953481 prime
139149151157 prime
141516182021 prime
141592653589 prime
142112242123 prime
142915724779 prime
145679876431 prime
152164007921 prime
157639024808
162536496481 prime
164736913905
166400805323 prime
166425493681 prime
166425813649 prime
171190210231 prime
171727482881 prime
172573565537 prime
179178463997
183208285259 prime
186125268239 prime
189795640003 prime
194036151289 prime
198765432101 prime
200560490131 prime
209210612221
220123456789 prime
232911191713 prime
235375676171 prime
239651440411 prime
248857367251 prime
251117233141 prime
252097800623 prime
255255255251 prime
257778788987 prime
260389232731 prime
262364233421 prime
274860381259 prime
275311670611 prime
293826343073 prime
298999999999 prime
308457624821 prime
315569251763 prime
316234143227 prime
328628315459 prime
333227777777 prime
334473276937 prime
334826628433
337016056721 prime
339126523890
340000000000
344980016453 prime
351725765537 prime
352573616771 prime
353373727757 prime
355323522737 prime
355711131719 prime
357111317847
381654729067 prime
393142151459 prime
411379717319 prime
445317119867 prime
454280348267 prime
507995698619 prime
512463676147 prime
526858348381 prime
539423223413 prime
547716131821 prime
553559562581 prime
555553332211 prime
555555555551 prime
555555577777 prime
576529484441 prime
581485876661 prime updated
597325496783 prime
608888888809 prime
608981813029 prime
619737131179 prime
655372571753 prime
655373525717 prime
657835997711 prime
662626111933 prime
666666555551 prime
674960891221 prime
689101181569 prime
698908090691 prime
700123456789 prime
701234567897 prime
737797911337 prime
743008370701
753352617167 prime
753676171253 prime
762394144181 prime
762939453127 prime
774630686237
799980626860
816425361649 prime
824633702441 prime
902659997773 prime
921023456789 prime
964991203151 prime
976543312183 prime
978058181203 prime
999998999999 prime
999999000001 prime
1000000000039 prime
1000000000063 prime
1000000001159 prime
1000000999999 prime
1000123457689 prime
1012345678901 prime
1024688864201 prime
1030507050301
1043677052928
1049179854847 prime
1050100100501 prime
1099511628401 prime
1110111110111 prime
1111111111111
1111111999999 prime
1111118111111 prime
1116295198451 prime
1123529253211 prime
1131313515313 prime
1135279341568
1169769749219 prime
1177209242304
1218131412181 prime
1226280710981 prime
1231507051321 prime
1252099518401 prime
1344409044431 prime
1346140268867 prime
1349623494409 prime
1398023584459 prime
1428571758241 prime
1431535622177 prime
1468986414689 prime
1497631652873 prime
1534139560403 prime
1534696030979 prime
1548008755920
1597233891353 prime
1618033308161 prime
1618033988749 prime
1625800359439 prime
1666666666661 prime
1732565537257 prime
1835211125381 prime
1888081808881 prime
1918960000861 prime
2004006004001 prime
2072005925466
2335557777577 prime
2504730781961
2748779069441 prime
2760889966651 prime
2822916096001 prime
2935561623745
2999999899999 prime
3059220303001 prime
3111111111113 prime
3238428376721 prime
3331414141333
3465253618401
3525357323233 prime
3531577135439 prime
3571113171931 prime
3664533202453 prime
3672424151449
3763863863761 prime
3973810409128
4224496855063
4643668177711 prime
4666666666667 prime
4913685912167 prime updated
5298696301807 prime new
5510987654321 prime
5555555555551 prime
5942636062289 prime
6056529316217 prime
6252893229398
6660000000001 prime
6746328388801 prime
6763998516837
6987191424553 prime
7134354660741
7142857142857 prime
7177111117717 prime
7184500054817 prime
7284717174827 prime
7625597484960
7625597485003 prime
7666466646667 prime
7777774777777 prime
7777775552353 prime
8284590452353 prime
9203225223029 prime
9740985827329 prime
9876543244501 prime
#endif


//	printf("Size of unsigned long long %ld \n",sizeof(i));
//	printf("Size of unsigned long long %ld \n",sizeof(j));
//	printf("Size of unsigned long long %ld \n",sizeof(long double));
}
