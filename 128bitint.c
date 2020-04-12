
//
//  main.c
//  rsa-break
//
//  Created by Apple on 24/12/18.
//  Copyright © 2018 Apple. All rights reserved.



char a[2048];
char b[2048];
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *moveBit(char *s){ // moves the binary string one bit to the left (like s<<1 operation)
    char bit='0';
    char temp='0';
    for(int i=127;i>=0;i--){
        temp = s[i];
        s[i]=bit;
        bit=temp;
    }
    return s;
}

char *bitSum(char *a, char *b){ // figures out the sum of two numbers bit by bit
    
    char rem = '0';
    char* sum = (char *) malloc(2048);
    
    for(int i=127;i>=0;i--){
        
        int s = (a[i]=='1') + (b[i]=='1') + (rem=='1');
        
        if (s==3){
            sum[i]='1';
            rem='1';
        }else if(s==2){
            sum[i]='0';
            rem='1';
        }else{
            sum[i] = s? '1':'0';
            rem='0';
        }
    }
    
    return sum;
}

char *mult(char *a, char *b){
    
    char* product = (char *) malloc(2048);
    
    for(int i=127;i>=0;i--){
        
        if(b[i]=='1')
            product = bitSum(product,a);
        a=moveBit(a);
        
    }
    return product;
}

int main(){
    
 
    
strcpy(a, "01111111111111111111111111111111101010001101100110100000011111111111111111111111111111111111111111111111111110111111001111111111");
strcpy(b, "01111111111111111111111111111111111111111111110111111001111111111111111111111111111111111111111111111111111111011111100111111111");
    
    printf("\n\n%s\n", mult(a,b));
    return 0;
}


