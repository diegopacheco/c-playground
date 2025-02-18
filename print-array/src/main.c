#include <stdio.h>

void main(){
  int arr[5] = {1, 2, 3, 4, 5};
  int *ptr = arr;
  printf("Array elements: %d %d %d %d %d\n", 
    *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4));
}
