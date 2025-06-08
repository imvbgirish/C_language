#ifndef BOOKINGDETAILS_H
#define BOOKINGDETAILS_H

#include "payment.h"

typedef struct bookingDetails{
    char type[5];
    char name[11];
    char contactNumber[12];
    char brand[13];
    char vehicleNum[10];
    int rentDuration;
    int rentPrice;
    char status[11];
    payment *payment;
}book;

#endif // BOOKINGDETAILS_H
