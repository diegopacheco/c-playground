#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person{
	char name[20];
	int age;
};

int main(){

	struct Person person={"Deniss Ritchie", 60};
	unsigned char *buffer=(char*)malloc(sizeof(person));
	int i;

	memcpy(buffer,(const unsigned char*)&person,sizeof(person));

	printf("Copied byte array is:\n");
	for(i=0;i<sizeof(person);i++)
		printf("%02X ",buffer[i]);
	printf("\n");

	free(buffer);
	return 0;
}
