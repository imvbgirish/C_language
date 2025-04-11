#include <stdio.h>

int main(){

int arr[25];
int i,*j;

j = &arr[0];
int smallest = arr[0];


printf("Enter 25 numbers\n");
for(int i=0; i<25; i++){
	
	printf("number %d: ",i+1);
	scanf("%d", &arr[i]);

}

for(int i=0; i<25; i++){
	if(*j<smallest){
		smallest = *j;
	}
	j++;
}

printf("Smallest: %d",smallest);


return 0;

}