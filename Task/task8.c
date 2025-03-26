#include <stdio.h>

void moveDuplicates(int arr[], int size){
	int i,j,k = 0;
	int duplicates[size];
	int count = 0;

	for(i = 1; i<size; i++){
		int isDuplicate = 0;
		for(k = 0; k<j; k++){
			if(arr[i] == arr[k]){
				isDuplicate = 1;		
				break;
			}
		}
	if(!isDuplicate){
		arr[j++] = arr[i];
	}else{
		duplicates[count++] = arr[i];
	}
   }

	for(i = 0; i<count; i++){
		arr[j++] = duplicates[i];
	}
}

int main(){

int arr[] ={1, 2, 3, 3, 4, 5};
int size = sizeof(arr)/sizeof(arr[0]);

moveDuplicates(arr, size);

printf("After Moving Duplicates: \n");

for(int i=0; i< size; i++){
	printf("%d ", arr[i]);
}

printf("\n");

return 0;
}