#include <stdio.h>

int main(){

int a = 10;
float b = 10.0000000;

if(a == b){
	printf("Hi");       //10.0000 ---> prints Hi
}else{
	printf("Bye");	   //10.01 -----> prints Bye
}

return 0;
}