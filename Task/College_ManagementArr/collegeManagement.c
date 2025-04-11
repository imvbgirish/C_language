#include "collegeManagement.h"
#include <stdio.h>
#include <stdlib.h>

void addStudent(struct CollegeManagement *college){
    int num,i;
    printf("Enter number of student to add: ");
    scanf("%d",&num);

    if(college->studentList == NULL){
        college->studentList = (struct Student *)malloc(num * sizeof(struct Student));
        if(college->studentList == NULL){
            printf("Memory allocation failed");
            return;
        }
        college->numStudent = num;
    }else{
        college->studentList = (struct Student *)realloc(college->studentList,(college->numStudent+num)*sizeof(struct Student));
        if(college->studentList == NULL){
            printf("Memory allocation failed");
        }
        college->numStudent += num;
    }

    for(i = college->numStudent - num; i < college->numStudent; i++){
        printf("student name %d: ", i + 1);
        scanf("%s", college->studentList[i].name);

        printf("student roll no %d: ", i + 1);
        scanf("%d", &college->studentList[i].rollNo);
    }
}

void displayStudent(struct CollegeManagement *college){
    int i;
    if(college->studentList == NULL){
        printf("No data to display\n");
    }else{
        for(i=0; i<college->numStudent; i++){
            printf("%d. Name %s\n", i + 1, college->studentList[i].name);
            printf("%d. Roll No %d\n", i + 1, college->studentList[i].rollNo);
        }
    }
}

void addStaff(struct CollegeManagement *college){
    int num,i;
    printf("Enter Number of staff to add: ");
    scanf("%d",&num);

    if(college->staffList == NULL){
        college->staffList = (struct Staff *)malloc(num * sizeof(struct Staff));
        if(college->staffList == NULL){
            printf("Memory allocation failed");
            return;
        }
        college->numStaff = num;
    }else{
        college->staffList = (struct Staff *)realloc(college->staffList,(college->numStaff+num)*sizeof(struct Staff));
        if(college->staffList == NULL){
            printf("Memory allocation failed");
            return;
        }
        college->numStaff += num;
    }

    for (i = college->numStaff - num; i < college->numStaff; i++) {
        printf("staff member %d: ", i + 1);
        scanf("%s", college->staffList[i].name);
    }
}

void displayStaff(struct CollegeManagement *college){
    int i;
    if(college->staffList == NULL){
        printf("No data to display\n");
    }else{
        for(i=0; i<college->numStaff; i++){
            printf("%d. Name %s\n", i + 1, college->staffList[i].name);
        }
    }
}
