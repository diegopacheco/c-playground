#include <stdio.h>

void main(){
  printf("Hello");
  printf("%s\n", __FILE__);
  show_code();
}

void show_code(){
  char c;
  FILE *fp = fopen(__FILE__, "r");
  do{
    c = fgetc(fp);
    putchar(c);
  } while (c != EOF);
  fclose(fp);
}
