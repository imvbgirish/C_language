#include <stdio.h>

int main(){

struct time{

int hours; int min; int sec;

};

struct time t1 = {4, 30, 50};

struct time *pt;

pt = &t1;

printf("%d", pt->sec);

return 0;
}