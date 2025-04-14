#include <stdio.h>
#include <stdlib.h>
//#include <string.h>


typedef struct{
    char city[10];
    int pincode;
}address;

typedef struct{
    char name[10];
    int phoneNumber;
    address a;
}person;

int compareStruct(const void *pa, const void *pb){
    const person *p1 = pa;
    const person *p2 = pb;
    //return strcmp(pa,pb);

    return p1->a.pincode - p2->a.pincode;
}

int main()
{
   // printf("%lld\n",sizeof(person));
   // printf("%lld\n",sizeof(address));

    person p[] = {{"Girish",123456,{"Bengaluru",678867}},
                 {"Shridhar",456782,{"Davangere",979976}},
                 {"Mithun",902899,{"Shimogga",973927}}};

   int size = sizeof(p)/sizeof(person);

    qsort(p,size,sizeof(person),compareStruct);

   for(int i=0; i<size; i++){
        printf("%s %d %s %d\n",p[i].name, p[i].phoneNumber, p[i].a.city, p[i].a.pincode);
   }

   // printf("%lld bytes",sizeof(p)); // 96 bytes
    return 0;
}
