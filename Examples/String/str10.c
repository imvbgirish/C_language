#include <stdio.h>

int main(){

char s[] = "Get organised! Learn C!!";
printf("%s\n",&s[2]);
printf("%s\n",s);
printf("%s\n",&s);
printf("%c\n",s[2]);

return 0;
}

output:

t organised! Learn C!!
Get organised! Learn C!!
Get organised! Learn C!!
t