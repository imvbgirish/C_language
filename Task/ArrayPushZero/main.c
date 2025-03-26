#include <stdio.h>

void arrPushZero(int arr[], int size,int count){

    for(int i=0; i<size; i++){
        if(arr[i]!=0){
            arr[count] = arr[i];
            count++;
        }
    }
    while(arr[count]!=size){
        arr[count++] = 0;
    }
}

int main()
{
    int arr[] = {1,3,0,4,0,5};
    int size = sizeof(arr)/sizeof(int);
    int count = 0;

    arrPushZero(arr,size,count);

    for(int i=0; i<size; i++){
        printf("%d ", arr[i]);
    }
    return 0;
}
