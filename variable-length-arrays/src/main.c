#include <stdio.h>

int* createArrayN(int n) {
    int* arr = malloc(n * sizeof(int));
    for(int i = 0; i < n; i++) arr[i] = i;
    return arr;
}

int main(){
  int* arr = createArrayN(5);
  for(int i = 0; i < 5; i++) {
      printf("%d ", arr[i]);
  }
  free(arr);
}
