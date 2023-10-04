#include <stdio.h>
#include <sys/syscall.h>

void main(){
   system("echo 'ls -lsa | wc -l' ; ls -lsa | wc -l");
   system("echo \"\\$JAVA_HOME=$JAVA_HOME\"");

   syscall(SYS_write, 1, "hello, world!\n", 14);
}
