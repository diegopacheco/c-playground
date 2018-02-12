#include <stdio.h>

int main(){
	printf("Hello\011world");
	printf("\n");
	printf("Hello\x09world");
	printf("\n");

	return 0;
}
