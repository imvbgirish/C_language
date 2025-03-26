#include <stdio.h>

int main(){

struct employee{

 char name[20]; int age; int salary;

};

struct employee e[2];       // Array of structures


//struct employee e = {"aaa",33, 44343};
//struct employee e = {"ddd", 22, 88765};

struct employee e1 = {"Girish", 24, 36000};
struct employee e2 = {"Doctor", 23, 35000};
struct employee e3 = {"Engineer", 24, 55000};
struct employee e4 = {"Lawyer", 26, 25000};

printf("%s\n", e1.name);
printf("%s\n", e2.name);
printf("%s\n", e3.name);
printf("%s\n", e4.name);

printf("%d\n", e->age);



return 0;
}