#include <stdio.h>

int main(){

char str1[] = "hello";
char *p = "Hello";

printf("%d\n", &p);

printf("%s\n",p);

printf("%c\n",str1[0]);

printf("%c\n",*p);

p = "bye";

printf("%d\n", &p);

printf("%s", p);

return 0;

}