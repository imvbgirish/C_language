#include <stdio.h>

void findPrime(int num);


int main(){

printf("Enter number: ");
int num;
scanf("%d",&num);

findPrime(num);

return 0;

}

void findPrime(int num){

	for(int i=1; i<=num; i++){
		if(num%i==0){
			printf("%d\n",i);
		}
	}
}

