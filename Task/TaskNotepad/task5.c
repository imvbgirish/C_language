//Check Palindrome

#include <stdio.h>

int main(){

int num = 98754175;
int orgNum = num;
int lastDigit;
int sum = 0;
int smallest;
int largest;



while(num>0){
	lastDigit = num%10;
	sum = (sum * 10) + lastDigit;
	num /= 10;	
}

if(orgNum == sum){
	printf("palindrome");
}else{
	printf("not palindrome");
}

return 0;

}