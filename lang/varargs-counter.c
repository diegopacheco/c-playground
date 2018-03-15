#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void my_varargs(int argc,const char *s, ...){
  va_list argp;
  va_start(argp, s);

  printf("Varargs: \n");
  for(int i=0; i<= argc-1; i++){
    char *buf = va_arg(argp, char *);
    if(argp){
      printf("Arg %d %s \n",i,buf);
    }else{
      /// this code never ever runs
      printf("Ops I thin you are a lier %d \n",i);
    }
  }
  va_end(argp);
}

int main(){
  my_varargs(2,"myhash1","A","B");
  my_varargs(3,"myhash1","A","B","C");
  my_varargs(4,"myhash1","A","B","C","D");
  //my_varargs(10,"myhash1","A","B","C","D"); // this would blow things up.
  return 0;
}
