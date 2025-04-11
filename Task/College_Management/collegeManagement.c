#include "collegeManagement.h"
#include <stdio.h>
#include <stdlib.h>

struct Student *newStudent;
struct Staff *newStaff;
int num,i;

void addStudent(struct CollegeManagement *college){

    printf("Enter number of students to add: ");
    scanf("%d",&num);
    i=1;
    while(i<=num){

        newStudent = (struct Student *)malloc(sizeof(struct Student));

        printf("Enter Student Name: ");
        scanf("%s",newStudent->name);

        printf("Enter Roll No: ");
        scanf("%d",&newStudent->rollNo);

        newStudent->next = NULL;

        if(college->studentList == NULL){
            college->studentList = newStudent;
        }else{
            struct Student *temp = college->studentList;
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = newStudent;
        }
        i++;
    }

}

void displayStudent(struct CollegeManagement *college){
    struct Student *temp = college->studentList;
    i=1;
    if(temp == NULL){
        printf("No data to display");
        return;
    }
    while(i<=num){
        while(temp != NULL){
            printf("\nStudent Name: %s",temp->name);
            printf(", Student Roll No: %d",temp->rollNo);
            temp = temp->next;
        }
        i++;
    }
}


void addStaff(struct CollegeManagement *college){

    printf("Enter number of staff to add: ");
    scanf("%d",&num);
    i=1;
    while(i<=num){

        newStaff = (struct Staff *)malloc(sizeof(struct Staff));

        printf("Enter Staff Name: ");
        scanf("%s",newStaff->name);

        newStaff->next = NULL;

        if(college->staffList == NULL){
            college->staffList = newStaff;
        }else{
            struct Staff *temp = college->staffList;
            while(temp->next!=NULL){
                temp = temp->next;
            }
            temp->next = newStaff;
        }
        i++;
    }
}

void displayStaff(struct CollegeManagement *college){
    struct Staff *temp = college->staffList;
    if(temp == NULL){
        printf("No Data to Display");
        return;
    }
    i=1;
    while(i<=num){
        while(temp != NULL){
            printf("\nStaff Name: %s",temp->name);
            temp = temp->next;
        }
        i++;
    }
}

