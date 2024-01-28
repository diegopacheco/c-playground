#include <stdio.h>

void main(){
  int num;
  char character;

  printf("Type a number AND a character and press enter: \n");
  scanf("%d %c", &num, &character);

  printf("Your number is: %d\n", num);
  printf("Your character is: %c\n", character);
}
