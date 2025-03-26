#include "staff.h"
#include "student.h"
#include <stdio.h>

int main()
{
    printf("1. Add Student Data and Display it\n2. Display Student Data and Display it\n");

    int choice;
    printf("\nEnter Choice: ");
    scanf("%d",&choice);

    switch(choice){
    case 1: addStudentData();
        displayStudent();
        break;

    case 2: addStaffData();
        displayStaff();
        break;
    }

    return 0;
}
