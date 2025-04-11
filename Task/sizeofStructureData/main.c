#include <stdio.h>

struct size{
    int a;
    // float b;
    char c;
    double d;
    char e;
};

int main()
{
    struct size s;
    printf("%d bytes\n", sizeof(s.a));
   //printf("%d bytes\n", sizeof(s.b));
    printf("%d bytes\n", sizeof(s.c));
    printf("%d bytes\n", sizeof(s.d));
    printf("%d bytes\n", sizeof(s.e));

    printf("\nstruct size: %d bytes\n",sizeof(struct size));
    return 0;
}
