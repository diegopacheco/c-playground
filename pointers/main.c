#include <stdio.h>

void main(){
    values_pointers();
}

void values_pointers(){
    int counter = 42;
    int *pointer = &counter;

    printf("value = %d \n",*pointer); 
    printf("ref   = %lu \n",&pointer);
    printf("ref   = %lu \n",&counter);
}