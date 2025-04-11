#include <stdio.h>

int main(){

char name[] = "Girish";
char *p;
p = name;

while(*p!='\0'){
	printf("%c", *p);
	p++;
}
	
return 0;
}