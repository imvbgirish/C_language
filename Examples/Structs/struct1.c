#include <stdio.h>

int main(){

struct book{
	char name;
	int price;
	int pages;
};

struct book b1;

printf("Enter names, prices & no of pages\n");
scanf("%c %d %d", &b1.name, &b1.price, &b1.pages);


printf("%c %d %d\n", b1.name, b1.price, b1.pages);


return 0;
}