#include <stdio.h>

#define ENG_US	1
#define ENG_UK	2
#define FRENCH	3
#define LANGUAGE ENG_UK

int main(){

	#if LANGUAGE==ENG_US
		printf("Selected language is: ENG_US\n");
	#elif LANGUAGE==ENG_UK
		printf("Selected language is: ENG_UK\n");
	#else
		printf("Selected language is: FRENCH\n");
	#endif

	return 0;
}
