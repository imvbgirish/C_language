#include <stdio.h>

int main(){

struct book{
	char name[10]; float price; int pages;
};

struct book b1 = {"basic",130.00,550};
struct book b2 = {"physics",150.80,800};

printf("%s %f %d\n",b1.name, b1.price, b1.pages);
return 0;
}