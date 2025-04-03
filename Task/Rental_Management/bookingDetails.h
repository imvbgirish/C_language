#ifndef BOOKINGDETAILS_H
#define BOOKINGDETAILS_H

typedef struct bookingDetails{
    char type[10];
    char name[10];
    int contactNumber;
    char brand[15];
    char vehicleNum[10];
    int rentDuration;
    int rentPrice;
}book;

#endif // BOOKINGDETAILS_H
