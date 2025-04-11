#include <stdio.h>

int Add(int n);

int main(){

int num;
printf("Enter 5 digit number: ");
scanf("%d", &num);

int res = Add(num);
printf("%d",res);

}

int Add(int n){
	int sum = 0;
	int lastDigit = 0;
	while(n!=0){
		lastDigit = n%10;
		sum = sum + lastDigit;
		n = n/10;
	}
	return sum;
}
				
		