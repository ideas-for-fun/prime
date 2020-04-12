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
	__uint128_t xprime;
	char tmp[10];
	int add = 0, less = 0;
	int useincrement = 0;


//	xprime = prime * 297;  // THIS IS IMPORTANT FOR FASTER BREAKS
	xprime = (__uint128_t) prime;
// Estimate the MULTIPLIER PROPERLY and YOU ARE DONE !!!
//	xprime = prime / 9;
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

	FILE *fp = fopen("./13-15digit-primes.txt","r");

	if (fp == NULL)
	{
		useincrement = 1;
	}
	else
	{
		useincrement = 0;
	}

	add = 1;
		
	gets(tmp);

	while (1)
	{

// Since prime is usually BIGGER Decrement prime

		reminder = product % xprime;
		iter++;
//		printf("Iteration %d with product prime reminder = %llu \n", iter, (unsigned long long) reminder);
		if (reminder == 0)
		{
			printf("Broken product with prime in %d iterations\n", iter);
			print_uint128(xprime);
			break;
		}
		if (add)
		{
			if (useincrement)
				xprime++;
			else
			{
getmoreprime:
				if(fgets(buffer, 1024, fp) != NULL)
				{
					clear_newline(buffer);
					xprime = (__uint128_t) strtoll(buffer, NULL, 10);
					if (flag)
					{
						;
					}
					else 
					{
						if (xprime < prime)
						{
							goto getmoreprime;
						}
					}
						
				}
				else
				{
					break;
					printf("*********** Primes file has finished ************ \n");
				}
			}


		}
		else if (less)
			xprime--;
	}
	fclose(fp);

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
	count = c2termcount ;

	for (i = 127 , j = 0, k = 0; count ; i--, j++, k++)
	{
		if (powers[i] == 1)
		{
			c2terms_powers[i] =  j ;
			printf("%d c2terms_powers[%d]\n", c2terms_powers[i], i);
			consec_c2terms[k] =  j ;
			count--;
		}
	}

// TESTING TILL HERE :
// 

	median = i;
// Compute c1terms second Populated from 32 to 0 in i 
	count = c1termcount ;

	for (    ; count ; i--, j++, k++)
	{
		if (powers[i] == 1)
		{
			c1terms_powers[i] = j / 8;
			printf("%d c1terms_powers[%d]\n", c1terms_powers[i], i);
			consec_c1terms[k] = j / 8;
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

	count = c2termcount;
	count_interval = 0;
	c2termcount = 0;
	for (i = 127 , j = 0, k = 0; count ; i--, j++, k++)
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

	count = c1termcount ;
	count_interval = 0;
	c1termcount = 0;
	median = i;
	printf("\n Median reset to %d \n", median);

	for (    ; count ; i--, j++, k++)
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

// So 63 will be LSBit and 0 will be the MSBit
// Compute MULTIPLICATION of C1 with C2 to get intermediate result

// TODO : BUG HERE : TODO : FIX IT  Its the next 20 lines

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
					printf("Creating the summation polynomial %d %d\n", k, i);
					printf("Creating the summation polynomial %d \n", c2terms_powers[i] + c1terms_powers[k]);
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

	convert_128bit_to_polynomial(jj);

	__uint128_t difference;
	if (jj > product)
		difference = jj - product;
	else
		difference = product - jj;
	printf("Difference Report = ");
	print_uint128(difference);

// Take the difference and convert it to a polynomial....

	diffconvert_128bit_to_polynomial(difference);
	static int justoncec1terms[128];

	for (kk = 0; kk < 128; kk++)
		justoncec1terms[kk] = 0;

	if (jj < product)
		;
	else 
		goto resumeit;

	for (kk = 127, jjj = 0 ; kk ; jjj++, kk--)
	{
		if (diffpowers[kk] == 1)
		{
			for (i = median; i ; i--)
			{
				if ((c1terms_powers[i]) && (jjj))
					c1terms_powers[i] += jjj / c1termcount;
			}

		}

	}
	goto resumeit;


// TODO :
// FOR NOW THIS DOESNT WORK SO WORK ON FIXING IT !!!
reducepowers:
	for (kk = 127, jjj = 0 ; kk ; jjj++, kk--)
	{
		if (diffpowers[kk] == 1)
		{
			for (i = median; i ; i--)
			{
				if ((c1terms_powers[i]) && (jjj))
					c1terms_powers[i] -= jjj / c1termcount;
			}

		}

	}
	goto resumeit;



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
					printf("Creating the summation polynomial %d %d\n", k, i);
					printf("Creating the summation polynomial %d \n", c2terms_powers[i] + c1terms_powers[k]);
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

//	__uint128_t difference;
	int computed_product_is_big = 0;
	if (jj > product)
	{
		difference = jj - product;
		computed_product_is_big = 1;
		printf("Difference Report = COMPUTED LARGER : ");
// Adjustment of the c1term based on string length of the product and the computed value
		print_uint128(difference);
		prodlen = print_uint128(product);
		complen = print_uint128(jj);
		difflen = complen - prodlen;
		biasterm = (((difflen) * 10));
		printf("\n\n Adjusted Primes after recalc\n");
// TODO : FIX THIS CODE. Derive a relationship and fix it.
		print_uint128(c1compute / biasterm);
//		print_uint128(c1compute * 3/biasterm);
	}
	else
	{
		difference = product - jj;
		computed_product_is_big = 0;
		printf("Difference Report = ORIGINAL LARGER : ");
		print_uint128(difference);
		prodlen = print_uint128(product);
		complen = print_uint128(jj);
		difflen = prodlen - complen;
		biasterm = ((difflen) * 10) ;
		printf("\n\n Adjusted Primes after recalc\n");
// TODO : FIX THIS CODE. Derive a relationship and fix it.
		print_uint128(c1compute *  biasterm);
//		print_uint128(c1compute * 3 * biasterm);
	}


// Recalculate the summation_polynomial and recompute C1 and C2...

	break_product(product, c1compute, difference, actual_prime, computed_product_is_big);
			

}
