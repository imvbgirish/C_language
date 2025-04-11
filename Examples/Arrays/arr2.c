#include <stdio.h>

int main(){

int i=3, *x;
float j=1.5, *y;
char k='c', *z;

printf("value of i = %d\n",i);
printf("value of j = %f\n",j);
printf("value of k = %c\n",k);

x = &i; y = &j; z = &k;

printf("Address of x = %u\n",x);
printf("Address of y = %u\n",y);
printf("Address of z = %u\n",z);

x++; y++; z++;

printf("new Address of x = %u\n",x);
printf("new Address of y = %u\n",y);
printf("new Address of z = %u\n",z);



return 0;
}