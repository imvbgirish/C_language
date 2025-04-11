#include <ctype.h>
#include <stdio.h>


char p[] = "The sixth sick sheik's sixth ship is sick";

int main()
{
    // char ch;
    // ch = getchar();
    // if(islower(ch))
    //     putchar(toupper(ch));
    // else
    //     putchar(tolower(ch));
    // return 0;



    // int i =2;
    // float f = 2.5367;
    // char str[] = "Life is like that";
    // printf("%4d\t%3.3f\t%4s\n",i,f,str);
    // return 0;


    // printf("more often than not \rthe person who\
    //         wins is the one who thinks he can!\n");



    int i = 0;
    while(p[i] != '\0'){
        putchar(p[i]);
        i++;
    }
    return 0;
}
