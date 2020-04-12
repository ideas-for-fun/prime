
/* 
 * calculates (a * b) % c taking into account that a * b might overflow 
 */
__uint128_t mulmod(__uint128_t a, __uint128_t b, __uint128_t mod)
{
    __uint128_t x = 0,y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {    
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}
/* 
 * modular exponentiation
 */
__uint128_t modulo(__uint128_t base, __uint128_t exponent, __uint128_t mod)
{
    __uint128_t x = 1;
    __uint128_t y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}
 
/*
 * Miller-Rabin Primality test, iteration signifies the accuracy
 */
int Miller(__uint128_t p,int iteration)
{
 
    int i;
    __uint128_t s;
    if (p < 2)
    {
        return 0;
    }
    if (p != 2 && p % 2==0)
    {
        return 0;
    }
     s = p - 1;
    while (s % 2 == 0)
    {
        s /= 2;
    }
    for (i = 0; i < iteration; i++)
    {
        __uint128_t a = rand() % (p - 1) + 1, temp = s;
        __uint128_t mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
        {
            return 0;
        }
    }
    return 1;
}

#ifdef MILLER_STANDALONE
//Main
int main()
{
    int iteration = 5;
    __uint128_t num;
    printf("Enter integer to test primality: ");
    scanf("%lld", &num);
    if ( Miller( num, iteration))
    {
        printf("\nNumber is prime :");
	print_uint128(num);
    }
    else
    {
        printf("\nNumber is NOT prime :");
	print_uint128(num);
    }
    return 0;
}
#endif

char buffer[1024];

void clear_newline(char *s)
{
	while (*s)
	{
		if (*s == '\n')
		{
			*s = '\0';
			break;
		}
		else
		{
			s++;
		}
	}
	return;
}


void break_product(__uint128_t product, __uint128_t prime, __uint128_t difference, __uint128_t actual_prime, int flag)
{
	int iter = 0;
	__uint128_t reminder;
	__uint128_t xprime, savedprime;
	char tmp[10];
	int add = 0, less = 0;
	int useincrement = 0;
	__uint128_t diffac;


	xprime = (__uint128_t) prime;
	printf("\n Original Product of primes :");
	print_uint128(product);
	printf("\nDifference : ");
	print_uint128(difference);
	printf("\nUnadjusted Prime :");
	print_uint128(prime);
	printf("\nAdjusted Prime :");
	print_uint128(xprime);
	printf("\nActual Prime :");
	print_uint128(actual_prime);
	fflush(stdout);

//	FILE *fp = fopen("./13-15digit-primes.txt","r");
	int len, plen, diffit, biasdiv, i = 0;
	static int justonce = 1;

	useincrement = 1;

// FIX THIS : Jan 06 2019

	if (!(xprime % 2))
		xprime++;
	else
		;
	printf("\nMade Odd Prime :");
	len = print_uint128(xprime);
	plen = print_uint128(product);

#ifdef DONTDOTHIS
	plen = plen / 2;
	if (len > plen)
	{
		diffit = len - plen;
		biasdiv = 1;
		for (i = 0 ; i < (diffit - 1) ; i++)
			biasdiv = biasdiv * 10;
		xprime = xprime / (biasdiv);
	}
#endif
	savedprime = xprime;

	printf("\nMade equal length Prime :");
	len = print_uint128(xprime);
	
	int countup = 0 , countdown = 0;

	printf("\nDifference between actual and computed :");
	if (actual_prime > xprime)
	{
		diffac = actual_prime - xprime;
		print_uint128(diffac);
	}
	else
	{
		diffac = xprime - actual_prime;
		print_uint128(diffac);
	}
	

	gets(tmp);


	while (1)
	{

// Since prime is usually BIGGER Decrement prime

//	goto beforemodtestdown;


		if (flag)  // Computed product is bigger
		{
modtest:
#ifdef DONTDOTHIS // Jan 06 2019
			if (justonce)
			{
				xprime += (difference / 2);
				justonce = 0;
				if (!(xprime % 2))
					xprime++;
				else
					;
			}
#endif

			if (Miller(xprime, 5)) 	
			{

				reminder = product % xprime;
				iter++;
				printf("Iteration %d with product prime reminder = %llu \n", iter, (unsigned long long) reminder);
				print_uint128(xprime);
				if (reminder == 0)
				{
					printf("Broken product with prime in %d iterations\n", iter);
					print_uint128(xprime);
					break;
				}
			} 

			{
// TRY MOVING UP 1000000 times

				xprime += 2;
				while(!Miller(xprime,5))
				{
					xprime += 2;
					countup++;
				}
				if (iter < 10000000)
					goto modtest;
				else
					;

beforemodtestdown:
				iter = 0;
				xprime = savedprime;
				justonce = 1;
#ifdef DONTDOTHIS
				if (justonce)
				{
					xprime -= (difference / 2);
					justonce = 0;
					if (!(xprime % 2))
						xprime++;
					else
						;
				}
#endif

// TRY MOVING DOWN 1000000 times
modtestdown:
				if (Miller(xprime, 5)) 	
				{
					reminder = product % xprime;
					iter++;
					printf("Iteration %d with product prime reminder = %llu \n", iter, (unsigned long long) reminder);
					print_uint128(xprime);
					if (reminder == 0)
					{
						printf("Broken product with prime in %d iterations\n", iter);
						print_uint128(xprime);
						break;
					}
				} 

				xprime -= 2;

				// Move to the next prime in sequence
				while (!Miller(xprime, 5))
				{
					xprime -= 2;
//					xprime -= 2;
					countdown++;
				}

				if (iter < 100000000)
					;
				else
					break;

				if (xprime > 1000)
					goto modtestdown;

				if ((xprime) <= 1000)
				{
					printf("\n******* Broke the zero barrier while decrementing\n");
					break;
				}
			}
		}
		else 
		{
modtest1:
			// Original product is bigger
			if (Miller(xprime, 5)) 	
			{

				reminder = product % xprime;
				iter++;
				printf("Iteration %d with product prime reminder = %llu \n", iter, (unsigned long long) reminder);
				print_uint128(xprime);
				if (reminder == 0)
				{
					printf("Broken product with prime in %d iterations\n", iter);
					print_uint128(xprime);
					break;
				}
			}

			{
				// Move to the next prime in sequence
				xprime += 2;
				while (!Miller(xprime, 5))
				{
					xprime += 2;
				}
				goto modtest1;
			}
		}
	}
		
}

void build_c1_and_c2(__uint128_t product, __uint128_t actual_prime)
{

// Initialize c1terms and c2terms

	int i = 0, j = 0, k = 0;
	int termcount = 0;
	int divparts = 0;
	int c1termcount = 0;
	int c2termcount = 0;
	int countc1 = 0, countc2 = 0, count = 0;
	int ii = 0, kk = 0, jjj = 0;
	int polycount = 0, xcount = 0;
	int median = 0;
	int interval = 0;
	int count_interval = 0;

// Need to convert the product to a polynomial

	convert_128bit_to_polynomial(product);

// PASSED TILL HERE..

	for (i = 0 ; i < 64 ; i++)
	{
		c1terms[i] = 0L;
		c2terms[i] = 0L;
		c1terms_powers[i] = 0; // Contains power div 2 terms
		c2terms_powers[i] = 0; // Contains power div 2 terms
		c1terms_compdiv[i] = 0;// Divisor of final c1term
		c2terms_compdiv[i] = 0;// Divisor of final c2term
		terms_compdiv[i] = 0;
		terms_compdiv[64+i] = 0;
		terms_compdiv[128+i] = 0;
		terms_compdiv[192+i] = 0;
		summation_polynomial[i] = 0;
		summation_polynomial[64+i] = 0;
		summation_polynomial[128+i] = 0;
		summation_polynomial[192+i] = 0;
	}

// Lookup powers and count the terms in P(x)
	termcount = 0;

	for (i = 0 ; i < 128 ; i++)
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
	else
	{
		c1termcount = c2termcount = divparts;
	}

	printf("\n\n c2termcount %d c1termcount %d \n\n", c2termcount, c1termcount);

// PASSED TILL HERE !!!
// Compute c1terms_powers and c2terms_powers in i in place 
// Compute also the c1, c2 compdiv terms as well in i in place.

// Compute c2terms first Populated from 63 to 33 in i
	count = c1termcount ;

	for (i = 127 , j = 0, k = 0; count ; i--, j++, k++)
	{
		if (powers[i] == 1)
		{
			c1terms_powers[i] =  j ;
			printf("%d c1terms_powers[%d]\n", c1terms_powers[i], i);
			consec_c1terms[k] =  j ;
			count--;
		}
	}

// TESTING TILL HERE :
// 

	median = i;
// Compute c1terms second Populated from 32 to 0 in i 
	count = c2termcount ;

	for (    ; count ; i--, j++, k++)
	{
		if (powers[i] == 1)
		{
			c2terms_powers[i] = j / 10;
			printf("%d c2terms_powers[%d]\n", c2terms_powers[i], i);
			consec_c2terms[k] = j / 10;
			count--;
		}
	}

// TESTING : IT PASSED TILL HERE !!!

// Get the sqrt of termcount
// Let this be called interval
// Take the highest powers in C1 and C2 terms_powers array
//	char tmp[10];

// TO BE FIXED : Algorithm needs polishing

// Get the sqrt of termcount
// Let this be called interval
// Take the highest powers in C1 and C2 terms_powers array
// TO BE FIXED : Algorithm needs polishing

	int interval_bias = 2;

	interval = sqrt((double) termcount) - interval_bias ;

	count = c1termcount;
	count_interval = 0;
	c1termcount = 0;
	for (i = 127 , j = 0, k = 0; count ; i--, j++, k++)
	{
		if ((powers[i] == 1) && (!count_interval))
		{
			count--;
			count_interval++;
			c1termcount++;
		}
		else
		{
			if (powers[i] == 1) 
			{
				count_interval++;
				c1terms_powers[i] = 0;
				if (count_interval >= interval)
				{
					count_interval = 0;
				}
				count--;
			}
		}
	}

	count = c2termcount ;
	count_interval = 0;
	c2termcount = 0;
	median = i;
	printf("\n Median reset to %d \n", median);

	for (    ; count ; i--, j++, k++)
	{
		if ((powers[i] == 1) && (!count_interval))
		{
			count--;
			count_interval++;
			c2termcount++;
		}
		else
		{
			if (powers[i] == 1) 
			{
				count_interval++;
				c2terms_powers[i] = 0;
				if (count_interval >= interval)
				{
					count_interval = 0;
				}
				count--;
			}
		}
	}

// So 63 will be LSBit and 0 will be the MSBit
// Compute MULTIPLICATION of C1 with C2 to get intermediate result

// TODO : BUG HERE : TODO : FIX IT  Its the next 20 lines

	count = c1termcount;
	polycount = 0;
	printf("\n\n %d is count \n", count);

	for (i = 127 ; count > 1 ; i--)
	{
		if (c1terms_powers[i])
		{
			xcount = c2termcount;
			for (k = median ; xcount ; k--) 
			{
				if (c2terms_powers[k])
				{
					printf("Creating the summation polynomial %d %d\n", k, i);
					printf("Creating the summation polynomial %d \n", c2terms_powers[k] + c1terms_powers[i]);
					summation_polynomial[(c2terms_powers[k] + c1terms_powers[i])] = 1;
					polycount++;
					xcount--;
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

// Calculate the Median in the new polynomial

// TODO : median calculation is NOT good enough.
// This NEED TO BE FIXED. TEST IT. GO ON FROM THERE.

// MEDIAN IS SEVERELY FLAWED :

	xcount = 0;

// If the number of c1 and c2 terms is huge then possibly the
// j / 2 logic may be screwed up. We need to draw a relationship between
// (1) number of terms (2) order of the terms (3) j value to be introduced in the above logic far above.

	printf("%d is the polycount \n", polycount);

// Set the median again

	int bias = 0;
	int count_median;
	count_median = polycount / 2;
//	count_median = count_median / 2;
	bias = 10 ;

	for ( i = 127 , ii = 0 ; i ; i--, ii++)
	{
		if (summation_polynomial[i] == 1) 
		{
			printf("summation polynomial i = %d, ii = %d, Median = %d\n", i, ii, count_median);
			if (xcount <= (count_median - bias)) // A bit biased adding a term to the lesser power side of the division
			{
				c2compute += (__uint128_t) pow((double) 2, (double) i); 
				printf("SumPoly C2compute : power %d iteration %d xcount %d\n", i, i, xcount);
				xcount++;
			}
			else 
			{
				c1compute += (__uint128_t) pow((double) 2, (double) i);
				printf("SumPoly C1compute : power %d iteration %d xcount %d\n", i, i, xcount);
				xcount++;
			}
		}
	}

	printf("\n\nxcount is the total number of terms in the summation_polynomial %d \n", xcount);

	convert_128bit_to_polynomial(product);

// Print c1compute and c2compute
	printf("\n\n C1 and C2 are as follows\n");
	printf("%d is return value \n", print_uint128(c1compute));
	printf("%d is return value \n", print_uint128(c2compute));

	__uint128_t jj = (__uint128_t) c1compute * c2compute ;

	printf ("P3 and P4 product is as per computation  : ");
	print_uint128(jj);
	printf ("P3 and P4 product is original product is : ");
	print_uint128(product);

	__uint128_t difference;
#ifdef DONTDOTHISATALL // Jan 06 2019

	convert_128bit_to_polynomial(jj);

	if (jj > product)
		difference = jj - product;
	else
		difference = product - jj;
	printf("Difference Report = ");
	print_uint128(difference);

// Take the difference and convert it to a polynomial....

	diffconvert_128bit_to_polynomial(difference);
	static int justoncec1terms[128];
	int county = 0;

	for (kk = 0; kk < 128; kk++)
		justoncec1terms[kk] = 0;

	if (jj < product)
		;
	else 
		goto reducepowers;

// FIX IT : TODO for Jan 06 2019.

	printf("\n ********** ENHANCE POWERS in c1terms **************\n");
	for (kk = 127, jjj = 0 ; kk ; jjj++, kk--)
	{
		if (diffpowers[kk] == 1)
		{
			printf("kk = %d, jjj = %d\n", kk, jjj);
			for (i = median; i ; i--)
			{
				if ((c1terms_powers[i]) && (jjj))
					c1terms_powers[i] += (jjj / c1termcount) ;
				printf("c1terms_powers[%d] value= %d\n", i, c1terms_powers[i]);
			}

		}

	}
	printf("\n ********** ENHANCE POWERS in c2terms **************\n");
	for (kk = 127, jjj = 0 ; kk ; jjj++, kk--)
	{
		if (diffpowers[kk] == 1)
		{
			printf("kk = %d, jjj = %d\n", kk, jjj);
			for (i = 127 ; i > median ; i--)
			{
				if ((c2terms_powers[i]) && (jjj))
					c2terms_powers[i] += (jjj / c2termcount) ;
				printf("c2terms_powers[%d] value= %d\n", i, c2terms_powers[i]);
			}

		}

	}
	goto resumeit;



// TODO :
// FOR NOW THIS DOESNT WORK SO WORK ON FIXING IT !!!
//  FIXING HERE : Jan 06 2019
reducepowers:
#ifdef DONTDOTHIS // Jan 06 2019
	printf("\n ********* REDUCE POWERS in c1terms ****************\n");
	for (kk = 127, jjj = 0 , county = 0 ; kk ; jjj++, kk--)
	{
		if (diffpowers[kk] == 1)
		{
			printf("kk = %d, jjj = %d, county = %d\n", kk, jjj, county);
			for (i = median; i ; i--)
			{
				if ((c1terms_powers[i]) && (jjj))
					c1terms_powers[i] -= (jjj / c1termcount) ; // Maybe play around with denom on the RHS
				printf("c1terms_powers[%d] value= %d\n", i, c1terms_powers[i]);
			}
			county++;
			if (county > 2)
				break;

		}

	}

	printf("\n ********* REDUCE POWERS in c2terms ****************\n");
	for (kk = 127, jjj = 0 , county = 0 ; kk ; jjj++, kk--)
	{
		if (diffpowers[kk] == 1)
		{
			printf("kk = %d, jjj = %d, county = %d\n", kk, jjj, county);
			for (i = 127; i > median ; i--)
			{
				if ((c2terms_powers[i]) && (jjj))
					c2terms_powers[i] -= (jjj / c2termcount) ; // Maybe play around with denom on the RHS
				printf("c2terms_powers[%d] value= %d\n", i, c2terms_powers[i]);
			}
			county++;
			if (county > 2)
				break;

		}

	}
	goto resumeit;
	fflush(stdout);
#endif



resumeit:
	count = c2termcount;
	polycount = 0;
	printf("\n\n %d is count \n", count);

	for (i = 127 ; count > 1 ; i--)
	{
		if (c2terms_powers[i])
		{
			xcount = c1termcount;
			for (k = median ; xcount ; k--) 
			{
				if (c1terms_powers[k])
				{
					printf("At resumeit : Creating the summation polynomial %d %d\n", k, i);
					printf("At resumeit : Creating the summation polynomial %d \n", c2terms_powers[i] + c1terms_powers[k]);
					summation_polynomial[(c2terms_powers[i] + c1terms_powers[k])] = 1;
					polycount++;
					xcount--;
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

// Calculate the Median in the new polynomial

// TODO : median calculation is NOT good enough.
// This NEED TO BE FIXED. TEST IT. GO ON FROM THERE.

// MEDIAN IS SEVERELY FLAWED :

	xcount = 0;

// If the number of c1 and c2 terms is huge then possibly the
// j / 2 logic may be screwed up. We need to draw a relationship between
// (1) number of terms (2) order of the terms (3) j value to be introduced in the above logic far above.

	printf("%d is the polycount \n", polycount);

// Set the median again

	bias = 0;
//	int count_median;

	count_median = polycount / 2;
//	count_median = count_median / 2;
	bias = 10 ;

	for ( i = 127 , ii = 0 ; i ; i--, ii++)
	{
		if (summation_polynomial[i] == 1) 
		{
			printf("summation polynomial i = %d, ii = %d, Median = %d\n", i, ii, count_median);
			if (xcount <= (count_median - bias)) // A bit biased adding a term to the lesser power side of the division
			{
				c2compute += (__uint128_t) pow((double) 2, (double) i); 
				printf("SumPoly C2compute : power %d iteration %d xcount %d\n", i, i, xcount);
				xcount++;
			}
			else 
			{
				c1compute += (__uint128_t) pow((double) 2, (double) i);
				printf("SumPoly C1compute : power %d iteration %d xcount %d\n", i, i, xcount);
				xcount++;
			}
		}
	}

	printf("\n\nxcount is the total number of terms in the summation_polynomial %d \n", xcount);

	convert_128bit_to_polynomial(product);
#endif

// Print c1compute and c2compute
	printf("\n\n C1 and C2 are as follows\n");
	printf("%d is return value \n", print_uint128(c1compute));
	printf("%d is return value \n", print_uint128(c2compute));
	jj = (__uint128_t) product / actual_prime;

	printf("\n\n Actual Primes from Original Product\n");
	print_uint128(jj);
	print_uint128(actual_prime);


	jj = (__uint128_t) c1compute * c2compute ;

	printf ("Px and Py product is as per computation  : ");
	print_uint128(jj);
	printf ("Px and Py product is original product is : ");
	print_uint128(product);

//	convert_128bit_to_polynomial(jj);
	int prodlen, complen, difflen, biasterm;

	__uint128_t divby ;
	double quotient;
	int computed_product_is_big = 0;
	__uint128_t diffac, nc1compute ;
	if (jj > product)
	{
		difference = jj - product;
		computed_product_is_big = 1;
		printf("Difference Report = COMPUTED LARGER : ");
// Adjustment of the c1term based on string length of the product and the computed value
		print_uint128(difference);
		divby = difference / c1compute;
		printf("Divby : ");
		print_uint128(divby);
		quotient = (double) difference / (double) jj ; // REFINE THIS - possible num,den are jj, product, difference.
		printf("Adjust quotient %f : ", quotient);

		nc1compute = c1compute * ((quotient)) ; // Run parallel threads one with this c1compute
		printf("\nParallel thread (1) Difference between actual and computed :");
		if (actual_prime > nc1compute)
		{
			diffac = actual_prime - nc1compute;
			print_uint128(diffac);
		}
		else
		{
			diffac = nc1compute - actual_prime;
			print_uint128(diffac);
		}
		c1compute = c1compute * (1 + (quotient)) ; // Run parallel threads another one with this c1compute
		printf("\nParallel thread (2) Difference between actual and computed :");
		if (actual_prime > nc1compute)
		{
			diffac = actual_prime - nc1compute;
			print_uint128(diffac);
		}
		else
		{
			diffac = nc1compute - actual_prime;
			print_uint128(diffac);
		}

		prodlen = print_uint128(product);
		complen = print_uint128(jj);
		difflen = complen - prodlen;
		biasterm = (((difflen) * 10));
//		printf("\n\n Adjusted Primes after recalc\n");
// TODO : FIX THIS CODE. Derive a relationship and fix it.
//		print_uint128(c1compute / biasterm);
//		print_uint128(c1compute * 3/biasterm);
	}
	else
	{
// TODO : FIX THIS PART LATER Jan 06 2019
// ************** THIS MAY REQUIRE REWORK FOR WHOLE ALGORITHM... Jan 06 2019

		difference = product - jj;
		computed_product_is_big = 0;
		printf("Difference Report = ORIGINAL LARGER : ");
		print_uint128(difference);

		quotient = (double)  jj / (double) difference ; // REFINE THIS - possible num,den are jj, product, difference.
		printf("Adjust quotient %f : ", quotient);

		nc1compute = c1compute * (1 + (quotient)) ; // Run parallel threads one with this c1compute

		printf("\nParallel thread (1) Difference between actual and computed :");
		if (actual_prime > nc1compute)
		{
			diffac = actual_prime - nc1compute;
			print_uint128(diffac);
		}
		else
		{
			diffac = nc1compute - actual_prime;
			print_uint128(diffac);
		}
		
#ifdef DONTDOTHIS // Jan 06 2016
		nc1compute = c1compute * (1 + (quotient)) ; // Run parallel threads another one with this c1compute
		printf("\nParallel thread (2) Difference between actual and computed :");
		if (actual_prime > nc1compute)
		{
			diffac = actual_prime - nc1compute;
			print_uint128(diffac);
		}
		else
		{
			diffac = nc1compute - actual_prime;
			print_uint128(diffac);
		}
#endif

		prodlen = print_uint128(product);
		complen = print_uint128(jj);
		difflen = prodlen - complen;
		biasterm = ((difflen) * 10) ;
//		printf("\n\n Adjusted Primes after recalc\n");
// TODO : FIX THIS CODE. Derive a relationship and fix it.
//		print_uint128(c1compute *  biasterm);
//		print_uint128(c1compute * 3 * biasterm);
	}


// Recalculate the summation_polynomial and recompute C1 and C2...

	break_product(product, nc1compute, difference, actual_prime, computed_product_is_big);
			

}
