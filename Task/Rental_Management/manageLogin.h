#ifndef MANAGELOGIN_H
#define MANAGELOGIN_H

#include "adminDetails.h"
#include "customerDetails.h"
#include "vehicleManagement.h"

typedef struct managelogin{
    admin *adminList;
    customer *customerList;
}login;

void readPersonData(login *login);
void addPersonData(login *login,char *name,char *contactNumber,char *emailID, char *password, char *isAdmin);

void displayAdminlist(login *login);
void displayCustomerlist(login *login);

void adminLogin(login *login);
void userLogin(login *login);

void registerAdmin(login *login);
void registerUser(login *login);

void writePersonData(login *login);

void adminLoginMenu(login *login);
void userLoginMenu(login *login);

void mainMenu(login *login);

void adminMenu(manageVehicle *vehicle);
void userMenu(manageVehicle *vehicle);

void readLoginData(login *login);


#endif // MANAGELOGIN_H
