#ifndef MANAGELOGIN_H
#define MANAGELOGIN_H

#include "adminDetails.h"
#include "customerDetails.h"

typedef struct managelogin{
    admin *adminList;
    customer *customerList;
}login;

void readPersonData(login *login);
void addPersonData(login *login,char *name,int contactNumber,char *emailID, int password, char *isAdmin);

void displayAdminlist(login *login);
void displayCustomerlist(login *login);

void adminLogin(login *login);
void userLogin(login *login);

void registerAdmin(login *login);
void registerUser(login *login);

void writePersonData(login *login);

void adminMenu(login *login);
void userMenu(login *login);

void adminLoginMenu(login *login);
void userLoginMenu(login *login);

#endif // MANAGELOGIN_H
