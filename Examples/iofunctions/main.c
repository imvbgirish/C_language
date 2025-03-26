#include <stdio.h>

int main()
{
    // int i =10;
    // char ch = 'A';
    // float a = 3.14;
    // char str[20];

    // printf("%d %c %f\n",i , ch, a);
    // sprintf(str,"%d %c %f",i , ch, a ); //writes to an array of characters in memory
    // printf("%s", str);


    // char ch;
    // printf("Enter a string: ");
    // ch = getchar();
    // putchar(ch);

    char str[20];
    printf("\nEnter a string: ");
    gets(str);
    puts(str);

    return 0;
}
