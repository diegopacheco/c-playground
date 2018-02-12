#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
	char name[20];
	int age;
};

int main() {

	struct Person person = { "Deniss Ritchie", 60 };

	//declare character buffer (byte array)
	unsigned char *buffer=(char*)malloc(sizeof(person));
	int i;

	//copying....
	memcpy(buffer,(const unsigned char*)&person,sizeof(person));

	//printing..
  printf("Size of Struct Person == %ld \n", sizeof(struct Person));
	printf("Copied byte array is:\n");
	for(i=0;i<sizeof(person);i++){
      printf("%02X ",buffer[i]);
      if (i<=19){
          printf("%c ",buffer[i]);
      }else{
          printf("%i ",buffer[i]);
      }
  }
	printf("\n");

	//freeing memory..
	free(buffer);
	return 0;
}
