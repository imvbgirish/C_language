#ifndef VEHICLEMANAGEMENT_H
#define VEHICLEMANAGEMENT_H

#include "bookingDetails.h"
#include "rentalBikes.h"
#include "rentalCars.h"

typedef struct vehicleManagement{
    bike *bikelist;
    car *carlist;
    book *bookinglist;
}manageVehicle;

void readVehicleData(manageVehicle *vehicle);
void addVehicleData(manageVehicle *vehicle,char *type, char *brand, char *model, char *status,int rentPrice);
void writeVehicleData(manageVehicle *vehicle);
void addVehicle(manageVehicle *vehicle);
void displayVehicle(manageVehicle *vehicle);
void deleteVehicle(manageVehicle *vehicle);
void updatePrice(manageVehicle *vehicle);
void rentVehicle(manageVehicle *vehicle);
void returnVehicle(manageVehicle *vehicle);
void displayRentalHistory(manageVehicle *vehicle);

#endif // VEHICLEMANAGEMENT_H
