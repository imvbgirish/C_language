#ifndef MANAGEAUDITORIUM_H
#define MANAGEAUDITORIUM_H

#include "auditorium.h"

typedef struct{
    auditorium *auditoriumList;
}manageAuditorium;

void readAuditoriums(manageAuditorium *manageAuditorium);
void addAuditoriums(manageAuditorium *manageAuditorium, char *auditoriumName, char *month, int bookDate);
void writeAuditoriums(manageAuditorium *manageAuditorium);
void displayAuditorium(manageAuditorium *manageAuditorium);
void bookAuditorium(manageAuditorium *manageAuditorium);
void showAvailable(manageAuditorium *manageAuditorium);
void daysinMonth(char *month);
char *monthName(int num);

#endif // MANAGEAUDITORIUM_H
