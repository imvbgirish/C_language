#include <stdio.h>

int main(){
	
	int i=1,j=1;
	for(;;){
	
	if(i>5)
		break;
	else
		j += 1;
	printf("%d\n",j);           
	i += j;
printf("%d\n",i);
return 0;       //if both return exists or only this exists, then o/p is 2,3
	
}
return 0;	//prints 2,3,3,6 if only this return 0 exits.
}