#include <stdio.h>

void printArray(int arr[], int size);

void main(){
  int arr[] = {1,2,3,4,5,6};

  printf("%p\n", arr);
  
  printf("Size of arr      : %ld\n", sizeof(arr));
  printf("Total ints in arr: %ld\n", sizeof(arr) / sizeof(int));
  
  printf("First element    : %d\n", arr[0]);
  printf("First element    : %d\n", 0[arr]);
  printf("Last element     : %d\n", arr[5]);
  printf("Last element     : %d\n", 5[arr]);

  printf("Printing array using function\n");
  printArray(arr, 6);
}

void printArray(int arr[], int size){
  for(int i = 0; i < size; i++){
    printf("%d ", i[arr]);
  }
  printf("\n");
}