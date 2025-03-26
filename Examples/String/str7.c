#include <stdio.h>

int main(){

char str1[] = "Hello";
char str2[10];

char *s = "Good Morning";
char *q;

q=s;
str2 = str1;     //error: assignment to expression with array type
puts(q);

return 0;
}