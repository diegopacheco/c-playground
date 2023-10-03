#include <stdio.h>

void main(){
    values_pointers();
    pointer_of_pointers();
}

void values_pointers(){
    int counter = 42;
    int *pointer = &counter;

    printf("value = %d \n",*pointer); 
    printf("ref   = %lu \n",&pointer);
    printf("ref   = %lu \n",&counter);
}

void pointer_of_pointers(){
    int counter = 42;
    int *pointer = &counter;
    int **pp = &pointer;

    printf("pp value = %d \n",**pp); 
    printf("pp ref   = %lu \n",&pp);
}