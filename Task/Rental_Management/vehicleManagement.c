#include "vehicleManagement.h"
#include <stdio.h>
#include <stdlib.h>

void readVehicleData(manageVehicle *vehicle){
    FILE *fp;
    fp = fopen("VehicleData.txt","r");

    if(fp == NULL){
        printf("File not found\n");
        exit(0);
    }

    char type[10];
    char brand[15];
    char model[15];
    char status[10];
    int rentPrice;

    while(fscanf(fp,"%s %s %s %s %d", type, brand, model, status, &rentPrice)!=EOF){
        addVehicleData(vehicle, type, brand, model, status, rentPrice);
    }
    fclose(fp);
}

void addVehicleData(manageVehicle *vehicle,char *type, char *brand, char *model, char *status,int rentPrice){
    if(vehicle->bikelist == NULL){
        vehicle->bikelist = (struct rentalBike *)malloc(1*sizeof(struct rentalBike));
    }
}
