#include <stdio.h>

int main(){

char str1[] = {'H','e','l','l','o','0'};  //'0' ------ o/p: Hello0Ç
char str2[] = "Hello";			 // 0 -------- o/p: Hello

printf("%s\n", str1);
printf("%s\n",str2);

return 0;
}