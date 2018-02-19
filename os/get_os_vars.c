#include <stdio.h>
#include <stdlib.h>

int main(){
  printf("Get Linux Env VARS in C\n");
  const char* s = getenv("PATH");
  printf("PATH :%s\n",(s!=NULL)? s : "getenv returned NULL");
}
