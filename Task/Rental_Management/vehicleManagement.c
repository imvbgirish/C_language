#include "vehicleManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numOfBikes = 0;
int numOfCars = 0;
void readVehicleData(manageVehicle *vehicle){
    FILE *fp;
    fp = fopen("VehicleData.txt","r");

    if(fp == NULL){
        printf("File not found\n");
        exit(0);
    }

    char type[10];
    char brand[20];
    char model[20];
    char vehicleNum[10];
    char status[20];
    int rentPrice;

    while(fscanf(fp,"%s %s %s %s %s %d", type, brand, model, vehicleNum, status, &rentPrice) !=EOF){
        addVehicleData(vehicle, type, brand, model, vehicleNum, status, rentPrice);
    }
    fclose(fp);
}

void addVehicleData(manageVehicle *vehicle, char *type, char *brand, char *model, char *vehicleNum, char *status,int rentPrice){
    if(strcasecmp(type,"Bike")==0){

        if(vehicle->bikelist == NULL){
            //printf("1");
            vehicle->bikelist = (struct rentalBike *)malloc(1*sizeof(struct rentalBike));
            if(vehicle->bikelist == NULL){
                printf("Memory allocation failed\n");
                return;
            }
        }else{
            //printf("2");
            vehicle->bikelist = (struct rentalBike *)realloc(vehicle->bikelist,(numOfBikes+1)*sizeof(struct rentalBike));
        }

        //printf("3");
        vehicle->bikelist[numOfBikes].bike = (struct rentVehicleDetails *)malloc(1*sizeof(struct rentVehicleDetails));
        if (vehicle->bikelist[numOfBikes].bike == NULL) {
            printf("Memory allocation failed for bike details\n");
            return;
        }
        //printf("4");
        strcpy(vehicle->bikelist[numOfBikes].bike->type,type);
        strcpy(vehicle->bikelist[numOfBikes].bike->brand,brand);
        strcpy(vehicle->bikelist[numOfBikes].bike->model,model);
        strcpy(vehicle->bikelist[numOfBikes].bike->vehicleNum,vehicleNum);
        strcpy(vehicle->bikelist[numOfBikes].bike->status,status);
        vehicle->bikelist[numOfBikes].bike->rentPrice = rentPrice;

        numOfBikes++;
    }

    else if(strcasecmp(type,"Car")==0){
        //printf("5");
        if(vehicle->carlist == NULL){
            vehicle->carlist = (struct rentalCar *)malloc(1*sizeof(struct rentalCar));
            if(vehicle->carlist == NULL){
                printf("Memory Allocation failed\n");
                return;
            }
        }
        else{
            //printf("6");
            vehicle->carlist = (struct rentalCar *)realloc(vehicle->carlist,(numOfCars+1)*sizeof(struct rentalCar));
        }

        //printf("7");
        vehicle->carlist[numOfCars].car = (struct rentVehicleDetails *)malloc(sizeof(struct rentVehicleDetails));
        if(vehicle->carlist[numOfCars].car == NULL){
            printf("Memory allocation failed for car details\n");
            return;
        }

        //printf("8");
        strcpy(vehicle->carlist[numOfCars].car->type,type);
        strcpy(vehicle->carlist[numOfCars].car->brand,brand);
        strcpy(vehicle->carlist[numOfCars].car->model,model);
        strcpy(vehicle->carlist[numOfCars].car->vehicleNum,vehicleNum);
        strcpy(vehicle->carlist[numOfCars].car->status,status);
        vehicle->carlist[numOfCars].car->rentPrice = rentPrice;

        numOfCars++;

    }else{
        printf("No data to display\n");
        return;
    }
}

void displayBikes(manageVehicle *vehicle){

    if(vehicle->bikelist == NULL){
        printf("No data to display\n");
        return;
    }else{
        printf("\n\tBrand\t\tModel\t\tVehicleNumber\t\tStatus\t\tRentPrice\n");
        printf("--------------------------------------------"
               "----------------------------------------------\n");
        for(int i=0; i<numOfBikes; i++){
            printf("%12s %17s %19s %21s %15d\n",
                   vehicle->bikelist[i].bike->brand,
                   vehicle->bikelist[i].bike->model,
                   vehicle->bikelist[i].bike->vehicleNum,
                   vehicle->bikelist[i].bike->status,
                   vehicle->bikelist[i].bike->rentPrice);
        }
    }
}

void displayCars(manageVehicle *vehicle){

    if(vehicle->carlist == NULL){
        printf("No data to display\n");
        return;
    }else{
        printf("\n\tBrand\t\tModel\t\tvehicleNumber\t\tStatus\t\tRentPrice\n");
        printf("--------------------------------------------"
               "----------------------------------------------\n");
        for(int i=0; i<numOfCars; i++){
            printf("%12s %17s %19s %21s %15d\n",
                   vehicle->carlist[i].car->brand,
                   vehicle->carlist[i].car->model,
                   vehicle->carlist[i].car->vehicleNum,
                   vehicle->carlist[i].car->status,
                   vehicle->carlist[i].car->rentPrice);
        }
    }
}

void addBike(manageVehicle *vehicle){
    printf("\nEnter bike details to add\n");
    printf("-------------------------------\n");

    char type[10];
    strcpy(type,"Bike");

    char brand[20];
    char model[20];
    char vehicleNum[10];
    char status[20];
    int rentPrice;

    printf("Enter brand: ");
    scanf("%s",brand);

    printf("Enter model: ");
    scanf("%s",model);

    printf("Enter vehicleNum: ");
    scanf("%s",vehicleNum);

    printf("Enter status: ");
    scanf("%s",status);

    printf("Enter rent price: ");
    scanf("%d",&rentPrice);

    addVehicleData(vehicle, type, brand, model, vehicleNum, status, rentPrice);
    printf("Bike details added.\n");
}

void addCar(manageVehicle *vehicle){
    printf("\nEnter car details to add\n");
    printf("-------------------------------\n");

    char type[10];
    strcpy(type,"Car");

    char brand[20];
    char model[20];
    char vehicleNum[10];
    char status[20];
    int rentPrice;

    printf("Enter brand: ");
    scanf("%s",brand);

    printf("Enter model: ");
    scanf("%s",model);

    printf("Enter vehicleNum: ");
    scanf("%s",vehicleNum);

    printf("Enter status: ");
    scanf("%s",status);

    printf("Enter rent price: ");
    scanf("%d",&rentPrice);

    addVehicleData(vehicle, type, brand, model, vehicleNum, status, rentPrice);
    printf("Car details added.\n");
}

void writeVehicleData(manageVehicle *vehicle){
    FILE *fp;
    fp = fopen("vehicleData.txt","w");
    int i;

    if(fp == NULL){
        printf("File not found\n");
        exit(0);
    }

    for(i=0; i<numOfBikes; i++){
        fprintf(fp,"%s %s %s %s %s %d\n",
                "Bike",
                vehicle->bikelist[i].bike->brand,
                vehicle->bikelist[i].bike->model,
                vehicle->bikelist[i].bike->vehicleNum,
                vehicle->bikelist[i].bike->status,
                vehicle->bikelist[i].bike->rentPrice);
    }

    for(i=0; i<numOfCars; i++){
        fprintf(fp,"%s %s %s %s %s %d\n",
                "Car",
                vehicle->carlist[i].car->brand,
                vehicle->carlist[i].car->model,
                vehicle->carlist[i].car->vehicleNum,
                vehicle->carlist[i].car->status,
                vehicle->carlist[i].car->rentPrice);
    }
    fclose(fp);
}

void searchBike(manageVehicle *vehicle){
    int found = 0;
    char brand[15];
    printf("\nSearch Bike \n");
    printf("-------------------------------\n");

    printf("Enter brand: ");
    scanf("%s",brand);


    if(vehicle->bikelist == NULL){
        printf("No data to search\n");
        return;
    }else{
        printf("\n\tBrand\t\tModel\t\tvehicleNumber\t\tStatus\t\tRentPrice\n");
        printf("--------------------------------------------"
               "----------------------------------------------\n");
        for(int i=0; i<numOfBikes; i++){
            if(strcasecmp(vehicle->bikelist[i].bike->brand,brand)==0){
                found = 1;
                printf("%12s %17s %19s %21s %15d\n",
                       vehicle->bikelist[i].bike->brand,
                       vehicle->bikelist[i].bike->model,
                       vehicle->bikelist[i].bike->vehicleNum,
                       vehicle->bikelist[i].bike->status,
                       vehicle->bikelist[i].bike->rentPrice);
            }
        }
    }
    if(!found){
        printf("Bike not found for given brand.");
        return;
    }
}

void searchCar(manageVehicle *vehicle){
    int found = 0;
    char brand[15];
    printf("\nSearch Car \n");
    printf("-------------------------------\n");

    printf("Enter brand: ");
    scanf("%s",brand);

    if(vehicle->carlist == NULL){
        printf("No data to search\n");
        return;
    }else{
        printf("\n\tBrand\t\tModel\t\tvehicleNumber\t\tStatus\t\tRentPrice\n");
        printf("--------------------------------------------"
               "----------------------------------------------\n");
        for(int i=0; i<numOfCars; i++){
            if(strcasecmp(vehicle->carlist[i].car->brand,brand)==0){
                found = 1;
                printf("%12s %17s %19s %21s %15d\n",
                       vehicle->carlist[i].car->brand,
                       vehicle->carlist[i].car->model,
                       vehicle->carlist[i].car->vehicleNum,
                       vehicle->carlist[i].car->status,
                       vehicle->carlist[i].car->rentPrice);
            }
        }
    }
    if(!found){
        printf("Car not found for given brand.");
        return;
    }
}

void updateBikePrice(manageVehicle *vehicle){
    int found = 0;
    int newPrice = 0;
    char brand[15];
    char model[15];

    printf("\nUpdate Bike Price \n");
    printf("-------------------------------\n");

    printf("Enter brand: ");
    scanf("%s",brand);

    printf("Enter model: ");
    scanf("%s",model);

    if(vehicle->bikelist == NULL){
        printf("No data to update\n");
        return;
    }else{
        for(int i=0; i<numOfBikes; i++){
            if(strcasecmp(vehicle->bikelist[i].bike->brand,brand)==0 &&
                strcasecmp(vehicle->bikelist[i].bike->model,model)==0){
                found = 1;
                printf("Enter price: ");
                scanf("%d",&newPrice);
                vehicle->bikelist[i].bike->rentPrice = newPrice;
                printf("Bike price updated...\n");
                return;
            }
        }
    }
    if(!found){
        printf("Bike not found for given brand and model.\n");
        return;
    }
}

void updateCarPrice(manageVehicle *vehicle){
    int found = 0;
    int newPrice = 0;
    char brand[15];
    char model[15];

    printf("\nUpdate Car Price \n");
    printf("-------------------------------\n");

    printf("Enter brand: ");
    scanf("%s",brand);

    printf("Enter model: ");
    scanf("%s",model);

    if(vehicle->carlist == NULL){
        printf("No data to update\n");
        return;
    }else{
        for(int i=0; i<numOfCars; i++){
            if(strcasecmp(vehicle->carlist[i].car->brand,brand)==0 &&
                strcasecmp(vehicle->carlist[i].car->model,model)==0){
                found = 1;
                printf("Enter price: ");
                scanf("%d",&newPrice);
                vehicle->carlist[i].car->rentPrice = newPrice;
                printf("Car price updated..\n");
                return;
            }
        }
    }
    if(!found){
        printf("Car not found for given brand and model.\n");
        return;
    }
}

void sortByPriceBike(manageVehicle *vehicle){
    int i,j;
    bike temp;

    if(vehicle->bikelist == NULL){
        printf("No data to sort\n");
        return;
    }
    for(i=0; i<numOfBikes; i++){
        for(j=i+1; j<numOfBikes; j++){
            if(vehicle->bikelist[i].bike->rentPrice > vehicle->bikelist[j].bike->rentPrice){
                temp = vehicle->bikelist[i];
                vehicle->bikelist[i] = vehicle->bikelist[j];
                vehicle->bikelist[j] = temp;
            }
        }
    }
}

void sortByPriceCar(manageVehicle *vehicle){
    int i,j;
    car ref;

    if(vehicle->carlist == NULL){
        printf("No data to sort\n");
        return;
    }
    for(i=0; i<numOfCars; i++){
        for(j=i+1; j<numOfCars; j++){
            if(vehicle->carlist[i].car->rentPrice > vehicle->carlist[j].car->rentPrice){
                ref = vehicle->carlist[i];
                vehicle->carlist[i] = vehicle->carlist[j];
                vehicle->carlist[j] = ref;
            }
        }
    }
}

int bookHistory = 0;
void readBookingData(manageVehicle *vehicle){
    FILE *fp;
    fp = fopen("BookingData.txt","r");

    if(fp == NULL){
        printf("File not found\n");
        exit(0);
    }

    char type[10];
    char name[10];
    char contactNumber[15];
    char brand[15];
    char vehicleNum[10];
    int rentDuration;
    int rentPrice;

    while(fscanf(fp,"%s %s %s %s %s %d %d", type, name, contactNumber, brand,
                  vehicleNum, &rentDuration, &rentPrice) !=EOF){
        addBookingData(vehicle, type, name, contactNumber, brand, vehicleNum, rentDuration, rentPrice);
    }
    fclose(fp);
}

void addBookingData(manageVehicle *vehicle,char *type, char *name, char *contactNumber, char *brand, char *vehicleNum, int rentDuration,int rentPrice){
    FILE *fp;
    fp = fopen("BookingData.txt","r");

    if(vehicle->bookinglist == NULL){
        vehicle->bookinglist = (struct bookingDetails *)malloc(1*sizeof(struct bookingDetails));
        if(fp == NULL){
            printf("Mmeory allocation failed.\n");
            return;
        }
    }else{
        vehicle->bookinglist = (struct bookingDetails *)realloc(vehicle->bookinglist,(bookHistory+1)*sizeof(struct bookingDetails));
    }

    strcpy(vehicle->bookinglist[bookHistory].type,type);
    strcpy(vehicle->bookinglist[bookHistory].name,name);
    strcpy(vehicle->bookinglist[bookHistory].contactNumber,contactNumber);
    strcpy(vehicle->bookinglist[bookHistory].brand,brand);
    strcpy(vehicle->bookinglist[bookHistory].vehicleNum,vehicleNum);
    vehicle->bookinglist[bookHistory].rentDuration = rentDuration;
    vehicle->bookinglist[bookHistory].rentPrice = rentPrice;

    bookHistory++;
}

void writeBookingData(manageVehicle *vehicle){
    FILE *fp;
    fp = fopen("BookingData.txt","w");

    if(fp == NULL){
        printf("File not found\n");
        exit(0);
    }

    for(int i=0; i<bookHistory; i++){
        fprintf(fp,"%s %s %s %s %s %d %d\n",
                vehicle->bookinglist[i].type,
                vehicle->bookinglist[i].name,
                vehicle->bookinglist[i].contactNumber,
                vehicle->bookinglist[i].brand,
                vehicle->bookinglist[i].vehicleNum,
                vehicle->bookinglist[i].rentDuration,
                vehicle->bookinglist[i].rentPrice);
    }
    fclose(fp);
}

void displayRentalHistory(manageVehicle *vehicle){

    if(vehicle->bookinglist == NULL){
        printf("No data to display\n");
        return;
    }else{
        printf("\n\tType\tName\tContactNumber\tBrand\tVehicleNumber\tRentDuration\tRentPrice\n");
        printf("---------------------------------------------"
               "---------------------------------------------\n");
        for(int i=0; i<bookHistory; i++){
            printf("%11s %10s %10s %10s %12s %11d %15d\n",
                   vehicle->bookinglist[i].type,
                   vehicle->bookinglist[i].name,
                   vehicle->bookinglist[i].contactNumber,
                   vehicle->bookinglist[i].brand,
                   vehicle->bookinglist[i].vehicleNum,
                   vehicle->bookinglist[i].rentDuration,
                   vehicle->bookinglist[i].rentPrice);
        }
    }
}

void rentBike(manageVehicle *vehicle){

    char name[10];
    char contactNumber[15];
    int rentDuration;

    char brand[15];
    char model[15];
    int found = 0;

    printf("\nRenting Bike \n");
    printf("-------------------------------\n");
    printf("Enter brand: ");
    scanf("%s",brand);

    printf("Enter model: ");
    scanf("%s",model);

    for(int i=0; i<numOfBikes; i++){
        if(strcasecmp(vehicle->bikelist[i].bike->brand,brand)==0 &&
            strcasecmp(vehicle->bikelist[i].bike->model,model)==0){
            found = 1;
            if(strcasecmp(vehicle->bikelist[i].bike->status,"Available")==0){
                printf("Enter your name: ");
                scanf("%s",name);

                printf("Enter contact number: ");
                scanf("%s",contactNumber);

                printf("Enter rent duration in days: ");
                scanf("%d",&rentDuration);

                printf("Bike Booked Successfully...\n");
                strcpy(vehicle->bikelist[i].bike->status,"Booked");
                addBookingData(vehicle,"Bike", name, contactNumber, brand,
                               vehicle->bikelist[i].bike->vehicleNum,
                               rentDuration,vehicle->bikelist[i].bike->rentPrice*rentDuration);
                break;
            }
            else if(strcasecmp(vehicle->bikelist[i].bike->status,"Booked")==0){
                printf("Unable to book, bike is already booked.\n");
                break;
            }
        }
    }
    if(!found){
        printf("Bike not found\n");
    }
}
void rentCar(manageVehicle *vehicle){

    char name[10];
    char contactNumber[15];
    int rentDuration;

    char brand[15];
    char model[15];
    int found = 0;

    printf("\nRenting Car \n");
    printf("-------------------------------\n");
    printf("Enter brand: ");
    scanf("%s",brand);

    printf("Enter model: ");
    scanf("%s",model);

    for(int i=0; i<numOfCars; i++){
        if(strcasecmp(vehicle->carlist[i].car->brand,brand)==0 &&
            strcasecmp(vehicle->carlist[i].car->model,model)==0){
            found = 1;
            if(strcasecmp(vehicle->carlist[i].car->status,"Available")==0){
                printf("Enter your name: ");
                scanf("%s",name);

                printf("Enter contact number: ");
                scanf("%s",contactNumber);

                printf("Enter rent duration in days: ");
                scanf("%d",&rentDuration);

                printf("Car Booked Successfully...\n");
                strcpy(vehicle->carlist[i].car->status,"Booked");
                addBookingData(vehicle,"Car", name, contactNumber, brand,
                               vehicle->carlist[i].car->vehicleNum,
                               rentDuration,vehicle->carlist[i].car->rentPrice*rentDuration);
                break;
            }
            else if(strcasecmp(vehicle->carlist[i].car->status,"Booked")==0){
                printf("Unable to book, Car is already booked.\n");
                break;
            }
        }
    }
    if(!found){
        printf("Car not found\n");
    }
}

void returnBike(manageVehicle *vehicle){
    char vehicleNumber[10];
    int found = 0;

    printf("\nReturn Vehicle\n");
    printf("-------------------------------\n");
    printf("Enter vehicle number: ");
    scanf("%s",vehicleNumber);

    if(vehicle->bikelist == NULL){
        printf("No vehicle to return.\n");
        return;
    }
    else{
        for(int i=0; i<numOfBikes; i++){
            if(strcasecmp(vehicle->bikelist[i].bike->vehicleNum,vehicleNumber)==0){
                found = 1;
                strcpy(vehicle->bikelist[i].bike->status,"Available");
                printf("Vehicle returned successfully.\n");
                return;
            }
        }
    }

    if(!found){
        printf("Bike not found.\n");
        return;
    }
}

void returnCar(manageVehicle *vehicle){
    char vehicleNumber[10];
    int found = 0;

    printf("\nReturn Vehicle\n");
    printf("-------------------------------\n");
    printf("Enter vehicle number: ");
    scanf("%s",vehicleNumber);

    if(vehicle->carlist == NULL){
        printf("No vehicle to return.\n");
        return;
    }
    else{
        for(int i=0; i<numOfCars; i++){
            if(strcasecmp(vehicle->carlist[i].car->vehicleNum,vehicleNumber)==0){
                found = 1;
                strcpy(vehicle->carlist[i].car->status,"Available");
                printf("Vehicle returned successfully.\n");
                return;
            }
        }
    }

    if(!found){
        printf("Car not found.\n");
        return;
    }
}

enum{
    Bike = 1,
    Car,
    Exit
};

void displayVehicle(manageVehicle *vehicle){
    int choice = 0;

    while(1){
        printf("\nDisplay Vehicles\n");
        printf("-------------------------------\n");
        printf("1.Bike\n2.Car\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case Bike:displayBikes(vehicle);
            break;

        case Car:displayCars(vehicle);
            break;

        case Exit:printf("Exiting...\n");
            return;

        default: printf("Invalid Choice\n");
            break;
        }
    }
}

void addVehicles(manageVehicle *vehicle){
    int choice = 0;

    while(1){
        printf("\nAdd Vehicles\n");
        printf("-------------------------------\n");
        printf("1.Bike\n2.Car\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case Bike:addBike(vehicle);
            break;

        case Car:addCar(vehicle);
            break;

        case Exit:printf("Exiting...\n");
            return;

        default: printf("Invalid Choice\n");
            break;
        }
    }
}

void searchVehicles(manageVehicle *vehicle){
    int choice = 0;

    while(1){
        printf("\nSearch Vehicles\n");
        printf("-------------------------------\n");
        printf("1.Bike\n2.Car\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case Bike:searchBike(vehicle);
            break;

        case Car:searchCar(vehicle);
            break;

        case Exit:printf("Exiting...\n");
            return;

        default: printf("Invalid Choice\n");
            break;
        }
    }
}

void sort(manageVehicle *vehicle){
    int choice = 0;

    while(1){
        printf("\nSort Vehicles\n");
        printf("-------------------------------\n");
        printf("1.Bike\n2.Car\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case Bike:sortByPriceBike(vehicle);
            displayBikes(vehicle);
            break;

        case Car:sortByPriceCar(vehicle);
            displayCars(vehicle);
            break;

        case Exit:printf("Exiting...\n");
            return;

        default: printf("Invalid Choice\n");
            break;
        }
    }
}

void updatePrice(manageVehicle *vehicle){
    int choice = 0;

    while(1){
        printf("\nUpdate Vehicle Price\n");
        printf("-------------------------------\n");
        printf("1.Bike\n2.Car\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case Bike:updateBikePrice(vehicle);
            break;

        case Car:updateCarPrice(vehicle);
            break;

        case Exit:printf("Exiting...\n");
            return;

        default: printf("Invalid Choice\n");
            break;
        }
    }
}

void rentVehicles(manageVehicle *vehicle){
    int choice = 0;

    while(1){
        printf("\nRent Vehicles\n");
        printf("-------------------------------\n");
        printf("1.Bike\n2.Car\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case Bike:rentBike(vehicle);
            break;

        case Car:rentCar(vehicle);
            break;

        case Exit:printf("Exiting...\n");
            return;

        default: printf("Invalid Choice\n");
            break;
        }
    }
}

void returnVehicles(manageVehicle *vehicle){
    int choice = 0;

    while(1){
        printf("\nReturn Vehicles\n");
        printf("-------------------------------\n");
        printf("1.Bike\n2.Car\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case Bike:returnBike(vehicle);
            break;

        case Car:returnCar(vehicle);
            break;

        case Exit:printf("Exiting...\n");
            return;

        default: printf("Invalid Choice\n");
            break;
        }
    }
}

void deleteVehicle(manageVehicle *vehicle){
    int choice = 0;

    while(1){
        printf("\nDelete Vehicles\n");
        printf("-------------------------------\n");
        printf("1.Bike\n2.Car\n3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case Bike:deleteBike(vehicle);
            break;

        case Car:deleteCar(vehicle);
            break;

        case Exit:printf("Exiting...\n");
            return;

        default: printf("Invalid Choice\n");
            break;
        }
    }
}

void deleteBike(manageVehicle *vehicle){
    char vehicleNum[10];
    int found = 0;

    printf("Delete Vehicle\n");
    printf("-------------------------------\n");
    printf("Enter vehicle number: ");
    scanf("%s",vehicleNum);

    if(vehicle->bikelist == NULL){
        printf("No bikes to delete.\n");
        return;
    }
    for(int i=0; i<numOfBikes; i++){
        if(strcasecmp(vehicle->bikelist[i].bike->vehicleNum,vehicleNum)==0){
            found = 1;
            free(vehicle->bikelist[i].bike);

            for(int j=i; j<numOfBikes-1; j++){
                vehicle->bikelist[j] = vehicle->bikelist[j+1];
            }
            numOfBikes--;
            printf("Bike deleted successfully.\n");
            break;
        }
    }
    if(!found){
        printf("No vehicle found.\n");
        return;
    }
}

void deleteCar(manageVehicle *vehicle){
    char vehicleNum[10];
    int found = 0;

    printf("Delete Vehicle\n");
    printf("-------------------------------\n");
    printf("Enter vehicle number: ");
    scanf("%s",vehicleNum);

    if(vehicle->carlist == NULL){
        printf("No bikes to delete.\n");
        return;
    }
    for(int i=0; i<numOfCars; i++){
        if(strcasecmp(vehicle->carlist[i].car->vehicleNum,vehicleNum)==0){
            found = 1;
            free(vehicle->carlist[i].car);

            for(int j=i; j<numOfCars-1; j++){
                vehicle->carlist[j] = vehicle->carlist[j+1];
            }
            numOfCars--;
            printf("Car deleted successfully.\n");
            break;
        }
    }
    if(!found){
        printf("No vehicle found.\n");
        return;
    }
}

void readAllData(manageVehicle *vehicle){
    vehicle->bikelist = NULL;
    vehicle->carlist = NULL;
    vehicle->bookinglist = NULL;

    readVehicleData(vehicle);
    readBookingData(vehicle);
}





