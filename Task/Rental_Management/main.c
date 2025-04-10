#include "manageLogin.h"
#include <string.h>

int main()
{
    login login;
    //manageVehicle vehicle;
    login.adminList = NULL;
    login.customerList = NULL;

    // readPersonData(&login);
    // displayAdminlist(&login);
    mainMenu(&login);

    return 0;
}
