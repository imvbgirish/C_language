#include <stdio.h>
#include <stdlib.h>

int main(){

int max, i, *p;
printf("Enter array size: ");
scanf("%d", &max);
p = (int *) malloc (max * sizeof(int));		//ADDRESS WILL GET ASSIDNED TO P AND THEN WE CAN USE P AS A NORMAL ARRAY

for(i=0; i<max; i++){
	
	p[i] = i*i;
	printf("%d\n", p[i]);

}

return 0;

}