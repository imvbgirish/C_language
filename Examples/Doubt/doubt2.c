#include <stdio.h>

int main(){

int i;

for(;i<5; i--){
	printf("Hi");   //if garbage value is 0 runs infinitely.
}			//if garbage value is greater than 5 never runs.


return 0;
}