#include <stdio.h>
#include <string.h>

int main(){

char str[] = "Girish";
char str1[10];
int length;

length = strlen(str);

printf("%d\n", length);

strcpy(str1,str);
puts(str1);

char s[] = "Girish";
char s1[] = " V B";

strcat(s,s1);
puts(s);

char s2[] = "Girish";

int k;
k = strcmp(str,s2);
printf("%d",k);

return 0;
}