#include <stdio.h>
#include <stdlib.h>

int compareArr(const void *pa,const void *pb){

    const int *p1 = pa;
    const int *p2 = pb;

    return *p1 - *p2;
}

int main()
{
    int arr[] = {19,2,5,1,86,34,99,23};

    int size = sizeof(arr)/sizeof(int);

    qsort(arr,size,sizeof(int),compareArr);

    //printf("%d bytes\n",sizeof(arr));

    // int *a;
    // int c = 10;
    // a = &c;
    // printf("%d\n",*a);   //prints value of address it holds.
    // int *b = a;

    // printf("%d\n",&c);
    // printf("%d\n",a);   //prints address of a
    // printf("%d",b);

    for(int i=0; i<size; i++){
        printf("%d ",arr[i]);
    }

    return 0;
}
