#include <stdio.h>

struct threeNum{
   int n1, n2, n3;
};

int main(){
   int n;
   struct threeNum num;
   FILE *fptr;

   if ((fptr = fopen("/tmp/temp.bin","wb")) == NULL){
       printf("Error! opening file");
       return 1;
   }

   for(n = 1; n < 5; ++n){
      num.n1 = n;
      num.n2 = 5;
      num.n3 = 5 + 1;
      fwrite(&num, sizeof(struct threeNum), 1, fptr);
   }
   fclose(fptr);

   return 0;
}
