#include <stdio.h>

int main(){

	int c = 3;
	switch(c){

	case '3':
		printf("silver\n");
		break;
	case 3:
		printf("gold\n");
		break;
	default:
		printf("win");
}
return 0;
}