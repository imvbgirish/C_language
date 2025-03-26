#ifndef COLLEGE_H
#define COLLEGE_H

#include "collegeManagement.h"

struct College{
    char clgName[20];
    struct CollegeManagement manage;
};

#endif // COLLEGE_H
