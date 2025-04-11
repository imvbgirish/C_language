#include <stdio.h>

void result(int *,float *,int,int,int);

int main(){

int maths,sci,ss;
int avg;
float percent;

printf("Enter Marks of 3 Subjects: ");
scanf("%d",&maths);
scanf("%d\n",&sci);
scanf("%d",&ss);

result(&avg, &percent, maths, sci, ss);

printf("Average Marks is %d\n",avg);
printf("Percentage is %f",percent);

return 0;
}

void result(int *r1,float *r2,int m1,int m2,int m3){

*r1 = (m1+m2+m3)/3;

*r2 = ((*r1)*100)/100;

}





