#include "manageAuditorium.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numofAuditorium = 0;
char *months[] = {"jan", "feb", "mar", "april", "may", "june","july", "aug", "sep", "oct", "nov","dec"};

void readAuditoriums(manageAuditorium *manageAuditorium){
    FILE *fp;
    fp = fopen("auditoriumData.txt","r");

    if(fp == NULL){
        printf("cannot open file");
        exit(0);
    }
    char auditoriumName[10];
    char month[12];
    int bookedDate;

    while(fscanf(fp, "%s %s %d", auditoriumName, month, &bookedDate) != EOF){
        addAuditoriums(manageAuditorium, auditoriumName, month, bookedDate);
    }
    fclose(fp);
}

void addAuditoriums(manageAuditorium *manageAuditorium, char *auditoriumName, char *month,int bookDate){
    if(manageAuditorium->auditoriumList == NULL){
        manageAuditorium->auditoriumList = (auditorium *)malloc(1*sizeof(auditorium));
        if(manageAuditorium->auditoriumList == NULL){
            printf("Memory allocation failed.\n");
            return;
        }
    }else{
        manageAuditorium->auditoriumList = (auditorium *)realloc
            (manageAuditorium->auditoriumList,(numofAuditorium+1)*sizeof(auditorium));
    }

    manageAuditorium->auditoriumList[numofAuditorium].calender = (calender *)malloc(1*sizeof(calender));
    if (manageAuditorium->auditoriumList[numofAuditorium].calender == NULL) {
        printf("Memory allocation failed for calender details\n");
        return;
    }
    strcpy(manageAuditorium->auditoriumList[numofAuditorium].auditoriumName,auditoriumName);
    strcpy(manageAuditorium->auditoriumList[numofAuditorium].calender->month,month);
    manageAuditorium->auditoriumList[numofAuditorium].calender->date = bookDate;
    numofAuditorium++;
}

void displayAuditorium(manageAuditorium *manageAuditorium){
    if(manageAuditorium->auditoriumList == NULL){
        printf("No data to display\n.");
        return;
    }
    else{
        printf("\nName\n");
        printf("-----------------\n");
        for(int i=0; i<numofAuditorium; i++){
            printf("%s\n",
                   manageAuditorium->auditoriumList[i].auditoriumName);
        }
    }
    return;
}

void writeAuditoriums(manageAuditorium *manageAuditorium){
    FILE *fp;
    fp = fopen("auditoriumData.txt","w");

    if(fp == NULL){
        printf("File not found\n");
        exit(0);
    }
    for(int i=0; i<numofAuditorium; i++){
        fprintf(fp,"%s %s %d\n",
                manageAuditorium->auditoriumList[i].auditoriumName,
                manageAuditorium->auditoriumList[i].calender->month,
                manageAuditorium->auditoriumList[i].calender->date);
    }
    fclose(fp);
}

void bookAuditorium(manageAuditorium *manageAuditorium){
    int found = 0;
    char name[10];
    int month = 0;
    int bookedDate = 0;

    printf("Auditorium Name: ");
    scanf("%s",name);

    for(int i=0; i<numofAuditorium; i++){
        if(strcasecmp(manageAuditorium->auditoriumList[i].auditoriumName,name)==0){
            found = 1;
            printf("Enter date: ");
            scanf("%d",&bookedDate);

            printf("Enter month: ");
            scanf("%d",&month);

            char *nameofMonth = monthName(month);
            // printf("%s",nameofMonth);
            if((bookedDate<=0 || bookedDate>30) && (month <= 0 || month >12)){
                printf("Invalid detail\n");
                return;
            }
            else if(manageAuditorium->auditoriumList[i].calender->date == bookedDate &&
                     strcasecmp(manageAuditorium->auditoriumList[i].calender->month,nameofMonth )==0){
                printf("Auditorium already booked for the given date and month.\n");
                return;
            }
            else if(manageAuditorium->auditoriumList[i].calender->date != bookedDate &&
                     strcasecmp(manageAuditorium->auditoriumList[i].calender->month,"null" )==0){
                manageAuditorium->auditoriumList[i].calender->date = bookedDate;
                strcpy(manageAuditorium->auditoriumList[i].calender->month,nameofMonth);
                //strcpy(manageAuditorium->auditoriumList[i].status,"Booked");
                printf("Booked Successfully\n");
                break;
            }
        }
    }
    if(!found){
        printf("No auditorium found.\n");
        return;
    }
}
char *monthName(int num){

    switch(num){
    case 1:return months[0];
        break;
    case 2:return months[1];
        break;
    case 3:return months[2];
        break;
    case 4:return months[3];
        break;
    case 5:return months[4];
        break;
    case 6:return months[5];
        break;
    case 7:return months[6];
        break;
    case 8:return months[7];
        break;
    case 9:return months[8];
        break;
    case 10:return months[9];
        break;
    case 11:return months[10];
        break;
    case 12:return months[11];
        break;
    default:return NULL;
    }
}

void showAvailable(manageAuditorium *manageAuditorium){
    int date;
    int month = 0;
    int found = 0;
    printf("Enter date: ");
    scanf("%d",&date);

    printf("Enter month: ");
    scanf("%d",&month);

    char *nameofMonth = monthName(month);
    printf("\n");

    for(int i=0; i<numofAuditorium; i++){
        if(manageAuditorium->auditoriumList[i].calender->date != date &&
            manageAuditorium->auditoriumList[i].calender->month != nameofMonth){
            found = 1;
            printf("%s\n",
                   manageAuditorium->auditoriumList[i].auditoriumName);
        }
    }
    if(!found){
        printf("No data found\n");
        return;
    }
}

void daysinMonth(char *month){

    printf("\n Mon  Tue  Wed  Thur  Fri  Sat  Sun\n");

    //printf("%s",month);
    if(strcasecmp(month,months[0])==0 || strcasecmp(month,months[2])==0 || strcasecmp(month,months[4])==0 ||
        strcasecmp(month,months[6])==0 || strcasecmp(month,months[7])==0 || strcasecmp(month,months[9])==0 ||
        strcasecmp(month,months[11])==0){
        int days = 31;
        for(int day=1; day<=days; day++){
            printf("%4d ",day);
            if(day%7 == 0){
                printf(" \n");
            }
        }
    }else if(strcasecmp(month,months[1])==0){
        int days = 28;
        for(int day=1; day<=days; day++){
            printf("%4d ",day);
            if(day%7 == 0){
                printf(" \n");
            }
        }
    }
    else{
        int days = 30;
        for(int day=1; day<=days; day++){
            printf("%4d ",day);
            if(day%7 == 0){
                printf(" \n");
            }
        }
    }
}
