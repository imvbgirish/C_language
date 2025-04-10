#ifndef BOOKINGDETAILS_H
#define BOOKINGDETAILS_H

#include "payment.h"

typedef struct bookingDetails{
    char type[20];
    char name[20];
    char contactNumber[10];
    char brand[15];
    char vehicleNum[15];
    int rentDuration;
    int rentPrice;
    char status[15];
    payment *payment;
}book;

#endif // BOOKINGDETAILS_H
