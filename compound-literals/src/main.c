#include <stdio.h>

struct Point {
    int x, y, z;
};

void main(){
  int *arr = (int[]){1, 2, 3, 4, 5};
  printf("%d\n", ((struct Point){.x = 10, .y = 20}).x);
}
