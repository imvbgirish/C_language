#include "manageLogin.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

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
    int contactNumber;
    char emailID[20];
    int password;
    char isAdmin[5];

    while(fscanf(fp,"%s %d %s %d %s", name, &contactNumber, emailID, &password, isAdmin) !=EOF){
        addPersonData(login, name, contactNumber, emailID, password, isAdmin);
    }
    fclose(fp);
}

void addPersonData(login *login,char *name,int contactNumber,char *emailID, int password, char *isAdmin){
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
        login->adminList[numOfAdmins].admin->contactNumber = contactNumber;
        strcpy(login->adminList[numOfAdmins].admin->emailID,emailID);
        login->adminList[numOfAdmins].admin->password = password;
        strcpy(login->adminList[numOfAdmins].admin->isAdmin,isAdmin);

        numOfAdmins++;
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
        login->customerList[numOfCustomers].customer->contactNumber = contactNumber;
        strcpy(login->customerList[numOfCustomers].customer->emailID,emailID);
        login->customerList[numOfCustomers].customer->password = password;
        strcpy(login->customerList[numOfCustomers].customer->isAdmin,isAdmin);

        numOfCustomers++;
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
            printf("%12s %18d %26s %18d\n",
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
            printf("%12s %18d %26s %18d\n",
                   login->customerList[i].customer->name,
                   login->customerList[i].customer->contactNumber,
                   login->customerList[i].customer->emailID,
                   login->customerList[i].customer->password);
        }
    }
}

void adminLogin(login *login){
    int contactNumber = 0;
    int password = 0;
    int found = 0;

    printf("Enter Contact Number: ");
    scanf("%d",&contactNumber);

    printf("Enter Password: ");
    scanf("%d",&password);

    if(login->adminList == NULL){
        printf("No data found\n");
        return;
    }else{
        for(int i=0; i<numOfAdmins; i++){
            if(login->adminList[i].admin->contactNumber == contactNumber &&
                login->adminList[i].admin->password == password){
                //adminMenu(login);
            }else{
                printf("Invalid Details\n");
                return;
            }
        }
    }
    if(!found){
        printf("Details not found\n");
        return;
    }
}

void customerLogin(login *login){
    int contactNumber = 0;
    int password = 0;
    int found = 0;

    printf("Enter Contact Number: ");
    scanf("%d",&contactNumber);

    printf("Enter Password: ");
    scanf("%d",&password);

    if(login->customerList == NULL){
        printf("No data found\n");
        return;
    }else{
        for(int i=0; i<numOfCustomers; i++){
            if(login->customerList[i].customer->contactNumber == contactNumber &&
                login->customerList[i].customer->password == password){
                //userMenu(login);
            }else{
                printf("Invalid Details\n");
                return;
            }
        }
    }
    if(!found){
        printf("Details not found\n");
        return;
    }
}

void registerAdmin(login *login){
    printf("\nRegister Admin \n");
    printf("--------------------------\n");

    char name[10];
    int contactNumber;
    char emailID[20];
    int password;
    char isAdmin[5];

    printf("Enter name: ");
    scanf("%s",name);

    printf("Enter contact Number: ");
    scanf("%d",&contactNumber);

    printf("Enter EmailID: ");
    scanf("%s",emailID);

    printf("Enter password: ");
    scanf("%d",&password);

    strcpy(isAdmin,"Yes");

    addPersonData(login, name, contactNumber, emailID, password, isAdmin);
}

void registerUser(login *login){
    printf("\nRegister User \n");
    printf("--------------------------\n");

    char name[10];
    int contactNumber;
    char emailID[20];
    int password;
    char isAdmin[5];

    printf("Enter name: ");
    scanf("%s",name);

    printf("Enter contact Number: ");
    scanf("%d",&contactNumber);

    printf("Enter EmailID: ");
    scanf("%s",emailID);

    printf("Enter password: ");
    scanf("%d",&password);

    strcpy(isAdmin,"No");

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
        fprintf(fp,"%s %d %s %d %s\n",
                login->adminList[i].admin->name,
                login->adminList[i].admin->contactNumber,
                login->adminList[i].admin->emailID,
                login->adminList[i].admin->password,
                login->adminList[i].admin->isAdmin);
    }

    for(i=0; i<numOfCustomers; i++){
        fprintf(fp,"%s %d %s %d %s\n",
                login->customerList[i].customer->name,
                login->customerList[i].customer->contactNumber,
                login->customerList[i].customer->emailID,
                login->customerList[i].customer->password,
                login->customerList[i].customer->isAdmin);
    }
    fclose(fp);
}
