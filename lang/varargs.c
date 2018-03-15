#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//
// More on...
// https://www.eskimo.com/~scs/cclass/int/sx11b.html
//

void myprintf(const char *fmt, ...){
  va_list argp;
  va_start(argp, fmt);

  int i;
  char *buf;
  const char *s;
  char fmtbuf[256];
  for(s = fmt; *s != '\0'; s++){
    if(*s != '%'){
      putchar(*s);
      continue;
    }
    switch(*++s){
      case 'c':
        i = va_arg(argp, int);
        putchar(i);
        break;
      case 's':
        buf = va_arg(argp, char *);
        fputs(buf, stdout);
        break;
      case '%':
        putchar('%');
        break;
  }
 }
 va_end(argp);
}

int main( ) {
  myprintf("This is a print test %s %s \n","1","2");
  myprintf("This is a print test %s %s %s \n","1","2","3");
  myprintf("This is a print test %s %s %s %s\n","1","2","3","4");
  return 0;
}
