#include <stdio.h>

void Jiyo(int,int);
int main(){

int p= 23, f =24;
Jiyo(&p,&f);
printf("%d %d\n",p,f);
return 0;
}

void Jiyo(int q,int g){

q = q+q;
g = g+g;

}

Result: 

point5.c: In function 'main':
point5.c:7:6: warning: passing argument 1 of 'Jiyo' makes integer from pointer without a cast [-Wint-conversion]
 Jiyo(&p,&f);
      ^
point5.c:3:6: note: expected 'int' but argument is of type 'int *'
 void Jiyo(int,int);
      ^~~~
point5.c:7:9: warning: passing argument 2 of 'Jiyo' makes integer from pointer without a cast [-Wint-conversion]
 Jiyo(&p,&f);
         ^
point5.c:3:6: note: expected 'int' but argument is of type 'int *'
 void Jiyo(int,int);
      ^~~~