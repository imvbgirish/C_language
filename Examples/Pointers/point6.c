#include <stdio.h>

void check(int);
int main(){

int k = 35,z;
z = check(k);
printf("%d\n",z);
return 0;

}

void check(m){
{
int m;
if(m>40)
    return (1);
else
    return (0);
}


result:

point6.c:7:3: error: void value not ignored as it ought to be
 z = check(k);
   ^
point6.c: In function 'check':
point6.c:13:6: warning: type of 'm' defaults to 'int' [-Wimplicit-int]
 void check(m){
      ^~~~~
point6.c:17:12: warning: 'return' with a value, in function returning void
     return (1);
            ^
point6.c:13:6: note: declared here
 void check(m){
      ^~~~~
point6.c:19:12: warning: 'return' with a value, in function returning void
     return (0);
            ^
point6.c:13:6: note: declared here
 void check(m){
      ^~~~~
point6.c:20:1: error: expected declaration or statement at end of input
 }
 ^