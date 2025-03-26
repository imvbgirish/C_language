#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp;

    fp = fopen("data.txt","w");

    if(fp == NULL){
        printf("Cannot open file");
        exit(1);
    }

    char str[] = "India won champions trophy";

    fputs(str,fp);

    fclose(fp);

    printf("Data inserted");

    return 0;
}
