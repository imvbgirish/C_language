#include <stdio.h>

int Sum(int num);

int main(){


int num;
printf("Enter num: ");
scanf("%d",&num);

int k = Sum(num);
printf("Sum of all numbers is: %d\n",k);


return 0;
}

int Sum(int num){

	int sum;
	if(num==1){
		return(1);
	}
	else{
		sum = num + Sum(num-1);
	}
return sum;
}

