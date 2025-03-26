#include <stdio.h>

void CircularMotion(int *,int *,int *);

int main(){

int x = 5;
int y = 8;
int z = 10;

CircularMotion(&x,&y,&z);

printf("x = %d\n",x);
printf("y = %d\n",y);
printf("z = %d\n",z);

return 0;

}

void CircularMotion(int *x,int *y,int *z){

    int a;
	
	 a = *x;
	*x = *z;
 	*z = *y;
 	*y = a;

}