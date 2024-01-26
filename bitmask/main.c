#include <stdio.h>
#include <stdbool.h> 

//
// Operators
//
// & (Bitwise AND Operator)
// | (Bitwise OR Operator)
// ^ (Bitwise XOR Operator)
// ~ (Bitwise NOT Operator)
// >> (RIght Shift Operator)
// << (Left Shift Operator)
//

// does odd using bitmask
bool isOdd(int i) {
    return i&1;
}

void main(){
  // 
  // odd  : 1, 3, 5, 7, or 9
  // even : 0, 2, 4, 6, 8, 10
  //
  bool odd=isOdd(1);
  printf("is 1 odd? %d\n",odd); // 1 (true)

  odd=isOdd(2);
  printf("is 2 odd? %d\n",odd); // 0 (false)
}
