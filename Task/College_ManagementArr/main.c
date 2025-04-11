#include <stdio.h>
#include <stdlib.h>
#include "collegeManagement.h"

int main()
{
    struct CollegeManagement college;
    college.studentList = NULL;
    college.staffList = NULL;

    int choice = 1;

    do{
        printf("\n1. To add Student\n");
        printf("2. To display Student\n");
        printf("3. To add Staff\n");
        printf("4. To display Staff\n");
        printf("5. To exit\n\n");
        printf("Enter Choice: ");
        scanf("%d",&choice);

        switch(choice){
        case 1: addStudent(&college);
            break;
        case 2: displayStudent(&college);
            break;
        case 3: addStaff(&college);
            break;
        case 4: displayStaff(&college);
            break;
        case 5: printf("Exiting\n");
            break;
        default: printf("Invalid Choice\n");
        }

    }while(choice != 5);

    free(college.staffList);
    college.staffList = NULL;

    free(college.studentList);
    college.studentList = NULL;

    return 0;
}
