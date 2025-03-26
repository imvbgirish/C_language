#include <stdio.h>

int main(){

int arr[] = {10,20,30,40,50,60};
int i = 4, *j, *k, *x, *y;

j = &i;
printf("%d\n", j);

j = j+9;
printf("%d\n", j);

k = &i;
printf("%d\n", k);

k = k-3;
printf("%d\n", k);

x = &arr[1];
printf("add of x %d\n",x);

y = &arr[5];
printf("add of y %d\n",y);


printf("%d\n", y-x);

j = &arr[4];
printf("%d\n", j);

k = (arr + 4);
printf("%d\n", k);


if(j==k)
	printf("The pointers point to the same location");
else
	printf("The pointers point to the diff location");
return 0;


}


output:

C:\Users\Girish V B\Desktop\Pthinks_work\Arrays>p
6422260
6422296
6422260
6422248
add of x 6422268
add of y 6422284
4
6422280
6422280
The pointers point to the same location