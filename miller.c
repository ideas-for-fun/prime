#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
