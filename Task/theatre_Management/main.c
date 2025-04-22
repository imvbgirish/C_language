#include "manageAuditorium.h"
#include <stdio.h>
#include <string.h>

int main()
{
    manageAuditorium auditorium;
    auditorium.auditoriumList = NULL;
    char *months[] = {"jan", "feb", "mar", "april", "may", "june", "july", "aug", "sep", "oct", "nov","dec"};

    readAuditoriums(&auditorium);
    daysinMonth(months[1]);

    printf("\n");
    int choice;

    enum{
        Display =1,
        Book,
        Available
    };

    while(1){
        printf("\n1.Display\n2.Book\n3.Available\n4.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice){
        case Display:displayAuditorium(&auditorium);
            break;

        case Book:bookAuditorium(&auditorium);
            break;

        case Available:showAvailable(&auditorium);
            break;

        case 4:printf("Exit...\n");
            writeAuditoriums(&auditorium);
            return 0;

        default:printf("Invalid choice\n");
        }
    }

    return 0;
}
