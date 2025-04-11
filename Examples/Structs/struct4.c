#include <stdio.h>
#include <string.h>

int main(){

struct employee{
	char name[10]; int age; 
};

struct employee e1 = {"Girish", 24};
struct employee e2, e3;

strcpy(e2.name, e1.name);
//e2.name = e1.name;           //Error
e2.age = e1.age;              // Works

e3 = e2;

printf("%s %d\n",e1.name, e1.age);
printf("%s %d\n",e2.name, e2.age);
printf("%s %d\n",e3.name, e3.age);


return 0;
}