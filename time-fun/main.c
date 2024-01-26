#include <stdio.h>
#include <time.h> 

void main(){
  time_t biggest = 0x7FFFFFFF; 
  printf("biggest = %s \n", ctime(&biggest) );

  time_t seconds;
  seconds = time(NULL);
  printf("Hours since January 1, 1970 = %ld\n", seconds/3600);

  struct tm *ts;
  time_t     now = time(NULL);
  char       buf[80];
  ts = localtime(&now);
  strftime(buf, sizeof(buf), "Now is == %a %Y-%m-%d %H:%M:%S %Z", ts);
  puts(buf);

}
