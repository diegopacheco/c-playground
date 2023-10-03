#include <stdio.h>
#include <stdlib.h>

void main(){
    alloc_free();   
    realloc_free();
}

void alloc_free(){
     char *str;
     str = (char *) malloc(16);

     strcpy(str, "c language rocks");
     printf("String = %s,  Address = %u\n", str, str);
     free(str);
}

void realloc_free(){
     char *str;
     str = (char *) malloc(16);
     str = (char *) realloc(str, 35);

    strcpy(str, "c language rocks");
     strcat(str, " de facto standard!");
     printf("String = %s,  Address = %u\n", str, str);
     free(str);
}