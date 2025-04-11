#include <stdio.h>

int CountSum(int num);

int main(){

int num;
printf("enter 5 digit number: ");
scanf("%d", &num);

int res;
res = CountSum(num);
printf("%d",res);

return 0;
}

int CountSum(int num){
	int lastDigit;
	int sum = 0;
	
	while(num>0){
		lastDigit = num%10;
		sum += lastDigit;
		num = num/10;
	}

return sum;
}
