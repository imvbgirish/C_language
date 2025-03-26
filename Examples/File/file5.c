#include <stdio.h>

int main(){

FILE *fp;
char ch;
int i=0;

fp = fopen("read.txt","r");
while((ch = fgetc (fp)) != EOF){
	printf("%c",ch);

 }

fclose(fp);
return 0;
}                                                                                                                                                                                                                                                                                                                                                                 