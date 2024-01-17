#include <stdio.h>

void main(){
  enum Level {
    LOW    = 40,
    MEDIUM = 70,
    HIGH   = 100
  };
  enum Level bloodborne = MEDIUM;
  printf("%d", bloodborne);
}
