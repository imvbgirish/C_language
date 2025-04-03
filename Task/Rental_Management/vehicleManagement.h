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
void addVehicleData(manageVehicle *vehicle, char *type, char *brand, char *model,
                    char *vehicleNum, char *status,int rentPrice);
void writeVehicleData(manageVehicle *vehicle);

void displayBikes(manageVehicle *vehicle);
void displayCars(manageVehicle *vehicle);

void addBike(manageVehicle *vehicle);
void addCar(manageVehicle *vehicle);

void searchBike(manageVehicle *vehicle);
void searchCar(manageVehicle *vehicle);

void sortByPrice(manageVehicle *vehicle);

void updateBikePrice(manageVehicle *vehicle);
void updateCarPrice(manageVehicle *vehicle);

void readBookingData(manageVehicle *vehicle);
void addBookingData(manageVehicle *vehicle,char *type, char *name, int contactNumber,
                    char *brand, char *vehicleNum, int rentDuration,int rentPrice);
void displayRentalHistory(manageVehicle *vehicle);
void writeBookingData(manageVehicle *vehicle);

void rentBike(manageVehicle *vehicle);
void rentCar(manageVehicle *vehicle);

void deleteVehicle(manageVehicle *vehicle);
void returnVehicle(manageVehicle *vehicle);

#endif // VEHICLEMANAGEMENT_H
