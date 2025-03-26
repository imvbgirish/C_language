#include <stdio.h>

int main(){
	
int temp;
scanf("%d", &temp);
switch(temp)
{
case(temp <=20):
	printf("cool\n");
case(temp>20 && temp<=30):           //DOES NOT REDUCE TO AN INTEGER (ERROR)
	printf("run\n");
case(temp>30 && temp<=40):
	printf("wish\n");
default:
	printf("good");
}
return 0;
}