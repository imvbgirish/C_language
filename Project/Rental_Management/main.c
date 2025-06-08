#include "manageLogin.h"
#include <string.h>

int main()
{
    login login;

    login.adminList = NULL;
    login.customerList = NULL;

    mainMenu(&login);

    return 0;
}
