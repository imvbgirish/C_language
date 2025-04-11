#include <stdio.h>

int main(){

int n;
printf("Enter array size: ");
scanf("%d", &n);

int arr[n];

printf("Enter %d numbers\n", n);

for(int i=0; i<n; i++){
	printf("number %d: ",i+1);
	scanf("%d", &arr[i]);				
}

for(int i=0,j=1; i<n; i=i+2,j=j+2){
	
	if(j==n){
		break;
	}
		
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
	
		
}

for(int i=0; i<n; i++){
	printf("%d ", arr[i]);
}

return 0;

}