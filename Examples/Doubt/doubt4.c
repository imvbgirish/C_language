#include <stdio.h>
#include <stdlib.h>

int main(){

int a;
printf("Enter number: ");
scanf("%d", &a);

switch(a){

case 1: printf("Hi");
	exit(0);     		//runs but include <stdlib.h>

case 2: printf("Hello");
	break;
}

return 0;
}