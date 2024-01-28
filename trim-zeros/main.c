#include <stdio.h> 
#include <string.h> 

void main() { 

    char* s = "00000012340"; 
    int n; 

    // strspn->Returns the length of the first segment of 
    // str1 that exclusively contains characters from str2. 
    if ((n = strspn(s, "0")) != 0 && s[n] != '\0') { 
        printf("%s", &s[n]); 
    } 
}