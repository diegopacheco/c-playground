#include <stdio.h>

// 64 bits, or larger
unsigned long long int factorial(unsigned int i){
    if (i<=1){
        return 1;
    }
    return i*factorial(i-1);
}

int main(){
    int i = 6;
    printf("Factorial of %d is %d\n", i, factorial(i));
    return 0;
}