#include <stdio.h>

int main(){

int prodSum = 1;
int countSum = 0;
int constSum = 0;
int num = 10;

for(int i=1; i<=30; i++){
	if(i<=10){
		prodSum *= i;
	 }
	else if(i<=20){
		countSum += i;
	}
	else{
		constSum += i + num;
	}
}

printf("%d\n",prodSum);
printf("%d\n",countSum);
printf("%d\n",constSum);


return 0;
}