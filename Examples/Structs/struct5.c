#include <stdio.h>

int main(){

struct address{
	char phone[12];
	char city[25];
	int pin;
};

struct emp{
	char name[25];
	struct address a;
};

struct emp e = {"Giri", "74112","Bnglr",10};
printf("name = %s\nphone = %s\n", e.name, e.a.phone);
printf("city = %s\npin = %d\n", e.a.city, e.a.pin);

return 0;
}