#include <stdio.h>

int main(){

int i=0;
i++;

if(i<=5){

	printf("C\n");
	exit (0);
	main();
}
return 0;

}