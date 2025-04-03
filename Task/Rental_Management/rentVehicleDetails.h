#ifndef RENTVEHICLEDETAILS_H
#define RENTVEHICLEDETAILS_H

typedef struct rentVehicleDetails{
    char type[10];
    char brand[15];
    char model[15];
    char vehicleNum[10];
    char status[10];
    int rentPrice;
}vehicleDetails;

#endif // RENTVEHICLEDETAILS_H
