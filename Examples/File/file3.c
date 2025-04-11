#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

FILE *fp;
char s[50];

fp = fopen("notes.txt","w");
if(fp == NULL){
	puts("cannot open file");
	exit(1);
}

printf("\nEnter few text:\n");
while(strlen(gets(s))>0){
	fputs(s, fp);
	fputs("\n",fp);
}
fclose(fp);


printf("\nreading file contents now...\n");
fp = fopen("notes.txt","r");
if(fp == NULL){
	puts("cannot open file");
	exit(2);
}
while(fgets(s,49,fp) != NULL)
	printf("%s", s);
fclose(fp);

return 0;
}