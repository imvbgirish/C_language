#ifndef VEHICLEMANAGEMENT_H
#define VEHICLEMANAGEMENT_H

#include "bookingDetails.h"
#include "rentalBikes.h"
#include "rentalCars.h"

typedef struct vehicleManagement{
    bike *bikelist;
    car *carlist;
    book *bookinglist;
    char loggedContact[12];
    char loggedName[11];
    char isAdmin[4];
}manageVehicle;

void readVehicleData(manageVehicle *vehicle);
void addVehicleData(manageVehicle *vehicle, char *type, char *brand, char *model,
                    char *vehicleNum, char *status,int rentPrice);
void writeVehicleData(manageVehicle *vehicle);

void displayVehicle(manageVehicle *vehicle);
void displayBikes(manageVehicle *vehicle);
void displayCars(manageVehicle *vehicle);

void addVehicles(manageVehicle *vehicle);
void addBike(manageVehicle *vehicle);
void addCar(manageVehicle *vehicle);

void searchVehicles(manageVehicle *vehicle);
void searchBike(manageVehicle *vehicle);
void searchCar(manageVehicle *vehicle);

void sort(manageVehicle *vehicle);
void sortByPriceBike(manageVehicle *vehicle);
void sortByPriceCar(manageVehicle *vehicle);

void updatePrice(manageVehicle *vehicle);
void updateBikePrice(manageVehicle *vehicle);
void updateCarPrice(manageVehicle *vehicle);

void readBookingData(manageVehicle *vehicle);
void addBookingData(manageVehicle *vehicle,char *type, char *name, char *contactNumber,
                    char *brand, char *vehicleNum, int rentDuration,int rentPrice,char *status,
                    char *paymentType, char *paymentID, char *paymentStatus,int  paidAmount);
void displayRentalHistory(manageVehicle *vehicle);
void writeBookingData(manageVehicle *vehicle);

void rentVehicles(manageVehicle *vehicle);
void rentBike(manageVehicle *vehicle);
void rentCar(manageVehicle *vehicle);

void returnVehicles(manageVehicle *vehicle);
void returnBike(manageVehicle *vehicle);
void returnCar(manageVehicle *vehicle);

void deleteVehicle(manageVehicle *vehicle);
void deleteBike(manageVehicle *vehicle);
void deleteCar(manageVehicle *vehicle);

void bikes(manageVehicle *vehicle);
void showAvailableBike(manageVehicle *vehicle);
void showBookedBike(manageVehicle *vehicle);

void cars(manageVehicle *vehicle);
void showAvailableCar(manageVehicle *vehicle);
void showBookedCar(manageVehicle *vehicle);

void readAllData(manageVehicle *vehicle);
void history(manageVehicle *vehicle);

void sortBikePriceQsort(manageVehicle *vehicle);
void sortCarPriceQsort(manageVehicle *vehicle);

#endif // VEHICLEMANAGEMENT_H
