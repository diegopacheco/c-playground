#include <stdio.h>
#include <stdarg.h>

double average ( int num, ... ){
    
    // Initializing arguments to store all values after num
    va_list arguments;                     
    va_start ( arguments, num );           

    double sum = 0;
    for ( int x = 0; x < num; x++ ){
        sum += va_arg ( arguments, double ); 
    }

    // Cleans up the list
    va_end ( arguments );                  
 
    return sum / num;                      
}

void main(){
  printf( "%f\n", average ( 3, 12.2, 22.3, 4.5 ) );
}
