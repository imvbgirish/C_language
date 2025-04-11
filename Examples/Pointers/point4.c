#include <stdio.h>
void pass(int, int);

int main(){

int i=135, a=135, k;
k=pass(i,a);
printf("%d\n",k);
return 0;
}

void pass(int j, int b)             //Error bcoz void does not return value
int c;
				    //int c is declared out of function block
{
c = j+b;
return(c);
}