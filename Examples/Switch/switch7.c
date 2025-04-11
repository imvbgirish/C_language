#include <stdio.h>

int main(){
int a =3,b =4,c ;
c = b-a;
switch(c){
	case 1||2:
		printf("God\n");
		break;
	case a||b:
		printf("Demon\n");         //Error 
		break;
}
return 0;
}
