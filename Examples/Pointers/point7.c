#include <stdio.h>

int function(int *);

int main(){

int i=35, *z;		// gives 37 as a result
			
			//if --- int i=25,*z;
			//z = function(&i);  error saying conversion from integer to point
z = function(&i);	//point7.c:11:3: warning: assignment makes pointer from integer without a cast [-Wint-conversion]
printf("%d\n",z);
return 0;

}

int function(int *m){

return (*m+2);
}