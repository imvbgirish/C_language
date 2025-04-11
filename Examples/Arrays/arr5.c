#include <stdio.h>

int main(){
	
int num[] = {2,6,3,6,7,4};
int i;

for(i =0 ; i<=5; i++){
	
printf("address = %u ", &num[i]);
printf("element = %d %d ", num[i], *(num+i));
printf("%d %d\n", *(i+num), i[num]);

}

return 0;
}

output:

C:\Users\Girish V B\Desktop\Pthinks_work\Arrays>p
address = 6422276 element = 2 2 2 2
address = 6422280 element = 6 6 6 6
address = 6422284 element = 3 3 3 3
address = 6422288 element = 6 6 6 6
address = 6422292 element = 7 7 7 7
address = 6422296 element = 4 4 4 4