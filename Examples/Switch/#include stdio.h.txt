#include <stdio.h>

int main(){

int ch = 'a'+'b';
switch(ch)
{
	case 'a':
	case 'b':
		printf("you\n");
	case 'A':
		printf("ashar\n");
	case 'b' + 'a':
		printf("a and b");
}
return 0;
}