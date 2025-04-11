#ifndef STUDENT_H
#define STUDENT_H

struct Student{
    char name[10];
    int rollNo;
    struct Student *next;
};

#endif // STUDENT_H
