#include <stdio.h>

int main(){

int s[4][2] = {
		{1234, 56},
		{1212, 26},
		{1434, 86},
		{1312, 96}
	};
int i;
for(i=0; i<=3; i++){
	printf("Address of %d th 1-D array = %u\n",i,s[i]);
}

return 0;
	
}

output:

Address of 0 th 1-D array = 6422268
Address of 1 th 1-D array = 6422276
Address of 2 th 1-D array = 6422284
Address of 3 th 1-D array = 6422292