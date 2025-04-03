#include "manageLogin.h"
#include "vehicleManagement.h"
#include <stdio.h>

int main()
{
    //printf("Hello World!\n");
    manageVehicle vehicle;
    vehicle.bikelist = NULL;
    vehicle.carlist = NULL;
    vehicle.bookinglist = NULL;

    login login;
    login.adminList = NULL;
    login.customerList = NULL;

    //readBookingData(&vehicle);
    //displayRentalHistory(&vehicle);

    //-----------------------------------------
    // readPersonData(&login);

    // displayAdminlist(&login);
    //displayCustomerlist(&login);

    //registerAdmin(&login);
    //registerUser(&login);

    //writePersonData(&login);

    // -----------------------------------------
    readVehicleData(&vehicle);

    displayBikes(&vehicle);
    //rentBike(&vehicle);

    // readBookingData(&vehicle);
    // displayRentalHistory(&vehicle);

    //displayCars(&vehicle);

    // addBike(&vehicle);
    // addCar(&vehicle);

    // updateBikePrice(&vehicle);
    // updateCarPrice(&vehicle);

    // displayBikes(&vehicle);

    // rentBike(&vehicle);
    // displayCars(&vehicle);

    // searchBike(&vehicle);
    // searchCar(&vehicle);

    //sortByPrice(&vehicle);

    //writeVehicleData(&vehicle);

    return 0;
}
