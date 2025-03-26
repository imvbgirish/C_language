#include <stdio.h>

void display1(int *, int);
void display2(int [], int);

int main(){

int num[] = {1,2,3,4,5};
display1(&num[0], 5);

printf("-------------------");
printf("\n");
				//display1(&num[0], 5); and display1(num, 5);  are same.
display2(num, 5);

return 0;
}

void display1(int *j, int n){
	int i;
	for(i=0; i<n; i++){
		printf("element %d\n", *j);
		j++;
	}

}

void display2(int j[], int n){          // j is still an integer pointer here
	int i;
	for(i=0; i<n; i++){
		printf("element %d\n", j[i]);
		
	}
}


output:

element 1
element 2
element 3
element 4
element 5
-------------------
element 1
element 2
element 3
element 4
element 5