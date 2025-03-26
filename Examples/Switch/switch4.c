#include <stdio.h>

int main(){

int k;                     //if it was float, then it will fail.                                    
float j = 2.0;
	switch(k=j+1){     //automatically converts to int and program runs.
	printf("%d",k);
	case 3:
		printf("trapped\n");
		break;
	default:
		printf("caught!\n");
}
return 0;
}
