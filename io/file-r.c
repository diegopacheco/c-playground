#include <stdio.h>

int main(){
   int num;
   FILE *fptr;

   if ((fptr = fopen("/tmp/temp.txt","r")) == NULL){
       printf("Error! opening file");
       return 1;
   }

   fscanf(fptr,"%d", &num);

   printf("Value of n=%d", num);
   fclose(fptr);

   return 0;
}
