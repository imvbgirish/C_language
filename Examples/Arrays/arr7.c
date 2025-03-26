#include <stdio.h>

int main(){

int arr[] = {2, 4, 3 ,5};
int rev[4];
int j=0;


for(int i=3; i>=0; i--){	
	rev[j] = arr[i];
	j++;
}

for(int j=0; j<=3; j++){
	printf("%d ", rev[j]);      // output: 5 3 4 2
}

return 0;

}