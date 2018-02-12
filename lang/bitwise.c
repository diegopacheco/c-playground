#include <stdio.h>

// shift 2 left  places, zeros are added as padding.
int mathPow(int number, int power){
  return number<<power;
}

// Shifts bits to of number div places to the right -- equivalent to integer division by 2^div
int mathDivideBy(int number, int div){
  return number>>div;
}

int main () {
  printf("mathPow(9,2)          == %d \n",mathPow(9,2));
  printf("mathDivideBy(10,2)    == %d \n",mathDivideBy(10,2));
  
  return 0;
}
