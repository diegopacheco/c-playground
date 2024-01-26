#include <stdio.h>
#include <stdlib.h>

//
// runs forever until it crash but 
// allocate all os memory - good todo chaos testing :-) 
//
void main() {
 int mb = 0;
 while ( malloc(1<<20)) ++mb;
 printf("Allocated %d Mb total\n", mb);
} 