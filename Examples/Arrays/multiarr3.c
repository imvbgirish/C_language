#include <stdio.h>

//pointer to an array

int main(){

int s[4][2] = {
		{12, 42},
		{16, 33},
		{14, 56},
		{19, 87}
	};

int (*p)[2];   		//Pointer to an array of two integers

int i, j, *pint;

for(i=0; i<=3; i++){
	
	p = &s[i];
	pint = (int *)p;

	printf("\n");

	for(j=0; j<=1; j++){
		printf("%d ",*(pint + j));
		
	}
	
}
return 0;
}