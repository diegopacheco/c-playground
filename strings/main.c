#include <stdio.h>
#include <string.h>

int main () {
   string_create();
   string_copy();
   
   char buffer[10] = {0};
   string_ref(&buffer);
   printf("String buffer is: [%s]\n", buffer);
   return 0;
}

void string_create(){
   char a[5] = {'H', 'e', 'l', 'l', 'o',};
   printf("String: %s\n", a );

   char b[5] = "Hello";
   printf("String: %s\n", b); 
}

void string_copy(){
    char str1[50] = "Hello";
    char str2[50] = "World!";
    char result[100];

    strcpy(result, str1);  
    strcat(result, " ");   
    strcat(result, str2);  
    printf("%s", result);
}

void string_ref(char *buf){
    buf[0] = 'O';
    buf[1] = 'K';
    buf[2] = '\0';
}