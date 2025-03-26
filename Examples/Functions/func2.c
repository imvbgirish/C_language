#include <stdio.h>

void message();

int main(){

message(message());
return 0;		//func2.c:7:1: error: invalid use of void expression  message(message());

}

void message(){

printf("hii");
}

