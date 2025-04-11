#include <stdio.h>

int main(){

int start;
printf("Enter start: ");
scanf("%d",&start);

int end;
printf("Enter End: ");
scanf("%d",&end);

int interval = end - start;
	

int prodSum = 1;
int countSum = 0;
int constSum = 0;
int num = 10;

if(interval == 30){

for(start; start<=end; start++){

	if(start <= end-20){
		prodSum *= start;
	}

	else if(start <= end-10){
		countSum += start;
	}

	else{
		constSum += start + num;
	}
}
	printf("%u\n",prodSum);
	printf("%d\n",countSum);
	printf("%d\n",constSum);
	}	
	
	else{
		printf("End");
	}

return 0;

}
