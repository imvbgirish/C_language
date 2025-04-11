#include <stdio.h>

int main(){

int a;
printf("Enter a number: ");
scanf("%d", &a);

while(1){

	switch(a){
		case 1: printf("Hi");
			return 0;                //only return will throw error 
						//return with any number doesn't throw any error

		case 2:
			printf("Hello");	//if a given value 2, runs infinitely
			break;
	}
}


return 0;
}