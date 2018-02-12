#include <stdio.h>

int main(){

  char myArray[] = { 0x61, 0x62, 0x63 };
  size_t myArraySize = sizeof(myArray);
  printf("Array Size %ld\n", myArraySize);

  for(int i=0;i<myArraySize;i++){
    printf("char[%c] int[%d] \n",myArray[i],myArray[i]);
  }

  return 0;
}
