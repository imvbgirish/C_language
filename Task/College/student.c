#include "student.h"
#include <stdio.h>
#include <stdlib.h>

int student_num;
struct Student *student;

void addStudentData(){

    printf("\nEnter Number of Students: ");
    scanf("%d", &student_num);

    student = (struct Student *)malloc(student_num * sizeof(struct Student));

    for(int i=0; i<student_num; i++){
        printf("Enter Name: ");
        scanf("%s", student[i].name);

        printf("Enter Roll No: ");
        scanf("%d", &student[i].roll_no);

        printf("\n");
    }

}

void displayStudent(){
    if(student_num == 0){
        puts("No data to display");
    }else{
        for(int i=0; i<student_num; i++){
            printf("Name: %s\n", student[i].name);
            printf("Roll No: %d\n", student[i].roll_no);
            printf("\n");
        }
    }
    free(student);
}
