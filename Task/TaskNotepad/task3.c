#include <stdio.h>

int main(){

int n;
printf("Enter array size: ");
scanf("%d", &n);

int arr[n];

printf("Enter %d numbers\n", n);

for(int i=0; i<n; i++){
	printf("number %d: ",i+1);
	scanf("%d", &arr[i]);				//storing data in an array
}

if(n%2==0){

	for(int i=0; i<n; i=i+2){
		int temp = arr[i];
		arr[i] = arr[i+1];
		arr[i+1] = temp;
	}

} else if(n%2!=0){

	for(int i=0; i<n-1; i=i+2){
		int temp = arr[i];
		arr[i] = arr[i+1];
		arr[i+1] = temp;
	}
}

	

for(int i=0; i<n; i++){
	printf("%d ", arr[i]);
}

return 0;

}