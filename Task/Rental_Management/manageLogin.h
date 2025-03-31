#ifndef MANAGELOGIN_H
#define MANAGELOGIN_H

#include "adminDetails.h"
#include "customerDetails.h"

typedef struct managelogin{
    admin *adminList;
    customer *customerList;
}login;

void adminMenu(login *login);
void userMenu(login *login);

void adminLoginMenu(login *login);
void userLoginMenu(login *login);

void adminLogin(login *login);
void userLogin(login *login);

void registerAdmin(login *login);
void registerUser(login *login);

#endif // MANAGELOGIN_H
