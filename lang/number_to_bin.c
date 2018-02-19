#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char* itob(int value) {
   char *bits = malloc(sizeof(char) + (CHAR_BIT + 1));
   for (int i = CHAR_BIT - 1; i >= 0; i -= 1) {
       bits[i] = '0' + (value & 0x01);
       value >>= 1;
   }
   bits[CHAR_BIT] = 0;
   return bits;
}

int main (){
  int i;
  char buffer [33];
  printf("Enter a number: ");
  scanf("%d",&i);

  sprintf(buffer, "%o", i);
  printf("octal is %s\n", buffer);

  sprintf(buffer, "%02x", i);
  printf("hexadecimal is %s\n", buffer);

  printf("binary is %s\n", itob(i));

  return 0;
}
