#include <stdio.h>

int main(){

int a;
printf("Enter number: ");
scanf("%d", &a);

switch(a){

case 1: printf("Hi");
	return 0;     		//it runs
				// if only return is there, then it says returning void to non void function.

case 2: printf("Hello");
	break;
}

return 0;
}
