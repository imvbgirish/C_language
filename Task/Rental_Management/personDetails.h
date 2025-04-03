#ifndef PERSONDETAILS_H
#define PERSONDETAILS_H

typedef struct personDetails{
    char name[10];
    int contactNumber;
    char emailID[20];
    int password;
    char isAdmin[5];
}person;

enum isAdmin {
  Yes,
    No
};

#endif // PERSONDETAILS_H
