#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char name[10];
    int rollNo;
    int marks;
}student;

int compare(const void *pa,const void *pb){
    return strcmp(pa,pb);  // ---> compares and returns int value to sort

    const student *s1 = pa;  //Type-casting pa to type student
    const student *s2 = pb;

    // return s1->marks - s2->marks;
    // return s1->rollNo - s2->rollNo;
}

int main()
{

    student s[] = {{"Girish",10,91},
                   {"Amith",5,37},
                   {"Mithun",7,80}};

    int n = sizeof(s)/sizeof(student);

    printf("%d bytes\n",sizeof(s));

    qsort(s,n, sizeof(student),compare);

    for(int i=0; i<n; i++){
        printf("%s %d %d\n",s[i].name,s[i].rollNo,s[i].marks);
    }
    //printf("%lld bytes",sizeof(student));
    return 0;
}
