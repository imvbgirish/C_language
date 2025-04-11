#include <stdio.h>

void linkfloat();

int main(){

struct book{
	char name[5];
	int pages;
};

struct book b[3];
int i;
int dh;

for(i=0; i<=2; i++){
	printf("Enter name and pages\n");
	scanf("%c %d",&b[i].name, &b[i].pages);
	while((dh = getchar()) != '\n');
}

for(i=0; i<=2; i++){
	printf("%c %d\n",b[i].name, b[i].pages);
}

return 0;

}


