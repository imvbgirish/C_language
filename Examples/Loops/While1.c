#include <stdio.h>

int main(){
	
int i=1,j=1;
	
while(i++<5){
	while(j++<10){
	if(j==8)
		break;
	else
		printf("%d %d\n",i,j);
	
	}
}
return 0;
}
		