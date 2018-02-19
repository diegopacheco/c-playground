#include <stdio.h>
#include <stdlib.h>

struct dateOfBirth{
	int dd,mm,yy;
};

struct studentDetails{
	char name[30];
	int age;
	struct dateOfBirth dob;
};

int main(){
  struct studentDetails std={"Mike",21,15,10,1990};
	printf("Name: %s, Age: %d\n",std.name,std.age);
	printf("DOB: %d/%d/%d\n",std.dob.dd,std.dob.mm,std.dob.yy);

  struct dateOfBirth dob={15,10,1990};
	struct studentDetails std2={"Mike",21,dob};
	printf("Name: %s, Age: %d\n",std2.name,std2.age);
	printf("DOB: %d/%d/%d\n",std2.dob.dd,std2.dob.mm,std2.dob.yy);

  return 0;
}
