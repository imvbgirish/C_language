#ifndef PAYMENT_H
#define PAYMENT_H

typedef struct paymentDetails{
    char paymentType[10];
    char paymentID[10];
    char paymentStatus[10];
    int paidAmount;
}payment;

#endif // PAYMENT_H
