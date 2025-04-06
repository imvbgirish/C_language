#ifndef BOOKINGDETAILS_H
#define BOOKINGDETAILS_H

typedef struct bookingDetails{
    char type[20];
    char name[20];
    char contactNumber[15];
    char brand[15];
    char vehicleNum[15];
    int rentDuration;
    int rentPrice;
}book;

#endif // BOOKINGDETAILS_H
