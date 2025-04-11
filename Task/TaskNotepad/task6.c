#include <stdio.h>

int main(){

int num;
printf("Enter number: ");
scanf("%d", &num);

int lastDigit;
int smallest = 0;
int largest = 0;

while(num>0){
	lastDigit = num%10;
	
	if(lastDigit <= smallest){
		smallest = lastDigit;
	}
	else if(lastDigit >= largest){
		largest = lastDigit;
	}
	
	num /= 10;	
}

printf("smallest: %u\n",smallest);
printf("largest: %u",largest);

return 0;

}