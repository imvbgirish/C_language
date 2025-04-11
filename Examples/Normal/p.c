#include <stdio.h>

int main(){

int salary;
printf("enter salary: \n");
scanf("%d", &salary);

int d_allowance = 0.4 * salary;
int r_allowance = 0.2 * salary;

int gross_salary = salary + (d_allowance + r_allowance);
printf("gross salary is: %d",gross_salary);

return 0;

}
