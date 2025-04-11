#ifndef COLLEGEMANAGEMENT_H
#define COLLEGEMANAGEMENT_H

#include "staff.h"
#include "student.h"

struct CollegeManagement{
    struct Student *studentList;
    struct Staff *staffList;
};

void addStudent(struct CollegeManagement *college);
void displayStudent(struct CollegeManagement *college);

void addStaff(struct CollegeManagement *college);
void displayStaff(struct CollegeManagement *college);

#endif // COLLEGEMANAGEMENT_H
