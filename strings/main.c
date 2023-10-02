#include <stdio.h>
#include <string.h>

int main () {
   string_create();
   string_copy();
   
   char buffer[10] = {0};
   string_ref(&buffer);
   printf("String buffer is: [%s]\n", buffer);

   char *buf = "ok";
   size_t length = 0;
   getline(&buf, &length, stdin);
   string_modify(&buf);
   printf("modified buffer is: [%s]\n", buf);
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

void string_modify(char **buf){
    // get the real size
    size_t size = strlen(*buf) -1;          
    printf("size: %d\n", size);

    *buf = realloc(*buf, size + 3);
	*(*buf + size + 0) = '!';
	*(*buf + size + 1) = '!';
	*(*buf + size + 2) = 0;
}