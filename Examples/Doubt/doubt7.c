#include <stdio.h>
#include <stdlib.h>

int main(){

int a;
printf("Enter a number: ");
scanf("%d", &a);

int i=0;

while(i<5){

	switch(a){
		case 1: printf("hi");
			exit (5);				//exit works, but it runs infinitely
							// exit(0) or any number also works correctly
		case 2: printf("Hello");		//if i++ is there and a value is 2, then it runs 5 times
			break;				// without i++, i value is always zero, hence it prints hello infinitely
		}
}

return 0;
}