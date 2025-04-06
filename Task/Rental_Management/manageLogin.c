#include "manageLogin.h"
#include "vehicleManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

manageVehicle vehicle;
int numOfAdmins = 0;
int numOfCustomers = 0;
void readPersonData(login *login){
    FILE *fp;
    fp = fopen("PersonData.txt","r");

    if(fp == NULL){
        printf("File not found\n");
        exit(0);
    }

    char name[10];
    char contactNumber[15];
    char emailID[20];
    char password[15];
    char isAdmin[5];

    while(fscanf(fp,"%s %s %s %s %s", name, contactNumber, emailID, password, isAdmin) !=EOF){
        addPersonData(login, name, contactNumber, emailID, password, isAdmin);
    }
    fclose(fp);
}

void addPersonData(login *login,char *name,char *contactNumber,char *emailID,char *password, char *isAdmin){
    if(strcasecmp(isAdmin,"Yes")==0){

        if(login->adminList == NULL){
            login->adminList = (struct adminDetails *)malloc(1*sizeof(struct adminDetails));
            if(login->adminList == NULL){
                printf("Memory Allocation failed\n");
                return;
            }
        }else{
            login->adminList = (struct adminDetails *)realloc(login->adminList,(numOfAdmins+1)*sizeof(struct adminDetails));
        }

        login->adminList[numOfAdmins].admin = (struct personDetails *)malloc(sizeof(struct personDetails));
        if (login->adminList[numOfAdmins].admin == NULL) {
            printf("Memory allocation failed for admin details\n");
            return;
        }

        strcpy(login->adminList[numOfAdmins].admin->name,name);
        strcpy(login->adminList[numOfAdmins].admin->contactNumber,contactNumber);
        strcpy(login->adminList[numOfAdmins].admin->emailID,emailID);
        strcpy(login->adminList[numOfAdmins].admin->password,password);
        strcpy(login->adminList[numOfAdmins].admin->isAdmin,isAdmin);

        numOfAdmins++;
        return;
    }
    else if(strcasecmp(isAdmin,"No")==0){

        if(login->customerList == NULL){
            login->customerList = (struct customerDetails *)malloc(1*sizeof(struct customerDetails));
            if(login->customerList == NULL){
                printf("Memory Allocation failed\n");
                return;
            }
        }else{
            login->customerList = (struct customerDetails *)realloc(login->customerList,(numOfCustomers+1)*sizeof(struct customerDetails));
        }

        login->customerList[numOfCustomers].customer = (struct personDetails *)malloc(sizeof(struct personDetails));
        if (login->customerList[numOfCustomers].customer == NULL) {
            printf("Memory allocation failed for customer details\n");
            return;
        }

        strcpy(login->customerList[numOfCustomers].customer->name,name);
        strcpy(login->customerList[numOfCustomers].customer->contactNumber,contactNumber);
        strcpy(login->customerList[numOfCustomers].customer->emailID,emailID);
        strcpy(login->customerList[numOfCustomers].customer->password,password);
        strcpy(login->customerList[numOfCustomers].customer->isAdmin,isAdmin);

        numOfCustomers++;
        return;

    }else{
        printf("No data to display\n");
        return;
    }
}

void displayAdminlist(login *login){
    int i;
    if(login->adminList == NULL){
        printf("No data to display\n");
        return;
    }else{
        printf("\n\tName\t\tContact\t\t\tEmailID\t\t\tPassword\n");
        printf("-----------------------------------------"
               "-----------------------------------------\n");
        for(i=0; i<numOfAdmins; i++){
            printf("%12s %18s %26s %18s\n",
                   login->adminList[i].admin->name,
                   login->adminList[i].admin->contactNumber,
                   login->adminList[i].admin->emailID,
                   login->adminList[i].admin->password);
        }
    }
}

void displayCustomerlist(login *login){
    int i;
    if(login->customerList == NULL){
        printf("No data to display\n");
        return;
    }else{
        printf("\n\tName\t\tContact\t\t\tEmailID\t\t\tPassword\n");
        printf("-----------------------------------------"
               "-----------------------------------------\n");;
        for(i=0; i<numOfCustomers; i++){
            printf("%12s %18s %26s %18s\n",
                   login->customerList[i].customer->name,
                   login->customerList[i].customer->contactNumber,
                   login->customerList[i].customer->emailID,
                   login->customerList[i].customer->password);
        }
    }
}

void registerAdmin(login *login){
    printf("\nRegister Admin \n");
    printf("--------------------------\n");

    char name[10];
    char contactNumber[15];
    char emailID[20];
    char password[15];
    char isAdmin[5];

    printf("Enter name: ");
    scanf("%s",name);

    printf("Enter contact Number: ");
    scanf("%s",contactNumber);

    printf("Enter EmailID: ");
    scanf("%s",emailID);

    printf("Enter password: ");
    scanf("%s",password);

    strcpy(isAdmin,"Yes");

    printf("Admin registered successfully...\n");
    addPersonData(login, name, contactNumber, emailID, password, isAdmin);
}

void adminLogin(login *login){
    char contactNumber[15];
    char password[15];
    int found =0;


    printf("\nLogin\n");
    printf("-------------------------------------\n");
    printf("Enter Contact Number: ");
    scanf("%s",contactNumber);

    if(login->adminList == NULL){
        printf("No data found\n");
        return;
    }else{
        for(int i=0; i<numOfAdmins; i++){
            if(strcasecmp(login->adminList[i].admin->contactNumber,contactNumber)==0){
                printf("Enter Password: ");
                scanf("%s",password);
                found = 1;
                if(strcasecmp(login->adminList[i].admin->password,password)==0){
                    printf("Successfull...\n");
                    adminMenu(&vehicle);
                    break;
                }else{
                    printf("Invalid password.\n");
                    return;
                }
            }
        }
    }
    if(!found){
        printf("Details not found\n");
        return;
    }
}

void userLogin(login *login){
    char contactNumber[15];
    char password[15];
    int found = 0;

    printf("\nLogin\n");
    printf("-------------------------------------\n");
    printf("Enter Contact Number: ");
    scanf("%s",contactNumber);

    if(login->customerList == NULL){
        printf("No data found\n");
        return;
    }else{
        for(int i=0; i<numOfCustomers; i++){
            if(strcasecmp(login->customerList[i].customer->contactNumber,contactNumber)==0){
                printf("Enter Password: ");
                scanf("%s",password);
                found = 1;
                if(strcasecmp(login->customerList[i].customer->password,password)==0){
                    printf("Successfull...\n");
                    userMenu(&vehicle);
                    break;
                }else{
                    printf("Invalid password.\n");
                    return;
                }
            }
        }
    }
    if(!found){
        printf("Details not found\n");
        return;
    }
}


void registerUser(login *login){
    printf("\nRegister User \n");
    printf("--------------------------\n");

    char name[10];
    char contactNumber[15];
    char emailID[20];
    char password[15];
    char isAdmin[5];

    printf("Enter name: ");
    scanf("%s",name);

    printf("Enter contact Number: ");
    scanf("%s",contactNumber);

    printf("Enter EmailID: ");
    scanf("%s",emailID);

    printf("Enter password: ");
    scanf("%s",password);

    strcpy(isAdmin,"No");

    printf("User registered successfully...\n");
    addPersonData(login, name, contactNumber, emailID, password, isAdmin);
}

void writePersonData(login *login){
    FILE *fp;
    fp = fopen("PersonData.txt","w");
    int i;

    if(fp == NULL){
        printf("File not found\n");
        exit(0);
    }

    for(i=0; i<numOfAdmins; i++){
        fprintf(fp,"%s %s %s %s %s\n",
                login->adminList[i].admin->name,
                login->adminList[i].admin->contactNumber,
                login->adminList[i].admin->emailID,
                login->adminList[i].admin->password,
                login->adminList[i].admin->isAdmin);
    }

    for(i=0; i<numOfCustomers; i++){
        fprintf(fp,"%s %s %s %s %s\n",
                login->customerList[i].customer->name,
                login->customerList[i].customer->contactNumber,
                login->customerList[i].customer->emailID,
                login->customerList[i].customer->password,
                login->customerList[i].customer->isAdmin);
    }
    fclose(fp);
}

enum{
    SignIn = 1,
    SignUp,
    Logout
};

void adminLoginMenu(login *login){
    int choice = 0;
    while(1){
        printf("\nAdmin Login Menu\n");
        printf("--------------------------\n");
        printf("1.SignIn\n2.SignUp\n3.LogOut\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case SignIn: adminLogin(login);
            break;

        case SignUp: registerAdmin(login);
            break;

        case Logout:
            printf("Logging out...\n");
            return;

        default:printf("Invalid choice\n");
            break;
        }
    }
}

void userLoginMenu(login *login){
    int choice = 0;
    while(1){
        printf("\nUser Login Menu\n");
        printf("--------------------------\n");
        printf("1.SignIn\n2.SignUp\n3.LogOut\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice){
        case SignIn: userLogin(login);
            break;

        case SignUp: registerUser(login);
            break;

        case Logout: printf("Logging out...\n");
            return;

        default:printf("Invalid choice\n");
            break;
        }
    }
}

enum {
    Admin = 1,
    User,
    Exit
};
void mainMenu(login *login){

    readLoginData(login);
    readAllData(&vehicle);

    int choice = 0;
    printf("Vehicle Rental Application\n");
   printf("--------------------------\n");
    while(1){
        printf("\nMain Menu\n");
        printf("--------------------------\n");
        printf("1.Admin\n2.User\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice){
        case Admin: adminLoginMenu(login);
            break;

        case User: userLoginMenu(login);
            break;

        case Exit: writeVehicleData(&vehicle);
            writeBookingData(&vehicle);
            writePersonData(login);
            free(vehicle.bikelist);
            free(vehicle.carlist);
            free(vehicle.bookinglist);
            free(login->adminList);
            free(login->customerList);
            printf("Exiting...\n");
            return;

        default:printf("Invalid choice\n");
            break;
        }
    }
}

enum{
    AddVehicle=1,
    DisplayVehicle,
    SearchVehicle,
    SortVehicle,
    UpdateVehicle,
    RentVehicle,
    ReturnVehicle,
    DeleteVehicle,
    DisplayRentalHistory,
    ExitAdmin
};

void adminMenu(manageVehicle *vehicle){

    int choice = 0;
    while(1){
        printf("\nAdmin Menu\n");
        printf("--------------------------\n");
        printf("1.Add Vehicle\n2.Display Vehicle\n3.Search Vehicle\n4.Sort Vehicle"
               "\n5.Update Vehicle Price\n6.Rent Vehicle\n7.Return Vehicle"
               "\n8.Delete Vehicle\n9.Display Rental History\n10.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch (choice) {
        case AddVehicle:addVehicles(vehicle);
            break;

        case DisplayVehicle:displayVehicle(vehicle);
            break;

        case SearchVehicle:searchVehicles(vehicle);
            break;

        case SortVehicle:sort(vehicle);
            break;

        case UpdateVehicle:updatePrice(vehicle);
            break;

        case RentVehicle: rentVehicles(vehicle);
            break;

        case ReturnVehicle: returnVehicles(vehicle);
            break;

        case DeleteVehicle: deleteVehicle(vehicle);
            break;

        case DisplayRentalHistory: displayRentalHistory(vehicle);
            break;

        case ExitAdmin: printf("Exiting...\n");
            return;

        default:printf("Invalid Choice\n");
            break;
        }
    }
}

enum{
    Display = 1,
    Search,
    Sort,
    Rent,
    Return,
    ExitUser
};

void userMenu(manageVehicle *vehicle){

    int choice = 0;
    while(1){
        printf("\nUser Menu\n");
        printf("--------------------------\n");
        printf("1.Display Vehicle\n2.Search Vehicle\n3.Sort Vehicle"
               "\n4.Rent Vehicle\n5.Return Vehicle"
               "\n6.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch (choice) {

        case Display:displayVehicle(vehicle);
            break;

        case Search:searchVehicles(vehicle);
            break;

        case Sort:sort(vehicle);
            break;

        case Rent: rentVehicles(vehicle);
            break;

        case Return: returnVehicles(vehicle);
            break;

        case ExitUser: printf("Exiting...\n");
            return;

        default:printf("Invalid Choice\n");
            break;
        }
    }
}

void readLoginData(login *login){
    login->adminList = NULL;
    login->customerList = NULL;

    readPersonData(login);
}


