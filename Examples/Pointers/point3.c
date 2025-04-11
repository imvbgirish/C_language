#include <stdio.h>

int main(){

float a =3.5;

float *b,*c;
b = &a;
c = b;

printf("%p %p %u\n",&a,b,c); 				// 6422292 same memory location
printf("%f %f %f %f %f\n",a, *(&a), *&a, *b, *c);   // all gives same value: 3.500000

return 0;
}