#include "WiFiManagement.h"
#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int num =0;
// void displayAllWifi(wifiManagement *wifi){
//     FILE *fp;
//     fp = fopen("data.txt","r");

//     if(fp == NULL){
//         printf("cannot open file");
//         exit(0);
//     }
//     if(wifi->wifiList == NULL){
//         wifi->wifiList = (struct wifi *)malloc(MAX*sizeof(struct wifi));
//         if(wifi->wifiList == NULL){
//             printf("Memory Allocation failed");
//             fclose(fp);
//             return;
//         }
//     }
//         while(fscanf(fp,"%s %d %s %d",wifi->wifiList[num].name, &wifi->wifiList[num].signalStrength,
//                       wifi->wifiList[num].status, &wifi->wifiList[num].password)!=EOF){
//         num++;
//         if(num>=MAX){
//             wifi->wifiList = (struct wifi *)realloc(wifi->wifiList,(num+1)* sizeof(struct wifi));
//         }
//         if(wifi->wifiList == NULL){
//             printf("Memory reallocation failed");
//             fclose(fp);
//             return;
//         }
//     }

//     fclose(fp);
//     free(wifi->wifiList);
// }


int WiFiSize = 0;
void readData(WiFiManagement *WiFi){
    FILE *fp;
    fp = fopen("data.txt","r");

    if(fp == NULL){
        printf("cannot open file");
        exit(0);
    }
    char name[10];
    int signalStrength;
    char status[10];
    int password;

    while (fscanf(fp, "%s %d %s %d", name, &signalStrength, status, &password) != EOF){
        //printf("inside while\n");
        add(WiFi, name, signalStrength, status, password);
        // num++;
    }
    fclose(fp);
}

void add(WiFiManagement *WiFi, char *name, int signalStrength, char *status, int password){
    if(WiFi->WiFiList == NULL){
        WiFi->WiFiList = (struct WiFi *)malloc(1*sizeof(struct WiFi));
        if(WiFi->WiFiList == NULL){
            printf("Memory Allocation failed");
            return;
        }
    }else{
        WiFi->WiFiList = (struct WiFi *)realloc(WiFi->WiFiList,(WiFiSize+1)* sizeof(struct WiFi));
    }


    strcpy(WiFi->WiFiList[WiFiSize].name,name);
    WiFi->WiFiList[WiFiSize].signalStrength = signalStrength;
    strcpy(WiFi->WiFiList[WiFiSize].status,status);
    WiFi->WiFiList[WiFiSize].password = password;

    WiFiSize++;
    //wifi->wifiSize++;
}

void displayWifi(WiFiManagement *WiFi){
    int i;
    sortWifi(WiFi);
    if(WiFi->WiFiList == NULL){
        printf("No wifi data to display\n");
        return;
    }else{
        printf("\tName\t\tSignalStrength\t\tstatus\t\tpassword\n\n");
        for(i=0; i<WiFiSize; i++){
            printf("%12s %15d %25s %15d\n",
                   WiFi->WiFiList[i].name,
                   WiFi->WiFiList[i].signalStrength,
                   WiFi->WiFiList[i].status,
                   WiFi->WiFiList[i].password);
            // printf("\nName: %s\n", wifi->wifiList[i].name);
            // printf("Signal Strength: %d\n", wifi->wifiList[i].signalStrength);
            // printf("Status: %s\n", wifi->wifiList[i].status);
            // printf("Password: %d\n", wifi->wifiList[i].password);
            // printf("\n");
        }
    }
}

void sortWifi(WiFiManagement *WiFi){
    int i,j;
    struct WiFi temp;
    for(i=0; i<WiFiSize; i++){
        for(j=i+1; j<WiFiSize; j++){

            if(strcasecmp(WiFi->WiFiList[i].status,"Connected")!=0
                && strcasecmp(WiFi->WiFiList[j].status,"Connected")==0){
                temp = WiFi->WiFiList[i];
                WiFi->WiFiList[i] = WiFi->WiFiList[j];
                WiFi->WiFiList[j] = temp;
            }

            if(strcasecmp(WiFi->WiFiList[i].status,"Available")==0
                && strcasecmp(WiFi->WiFiList[j].status,"Saved")==0){
                temp = WiFi->WiFiList[i];
                WiFi->WiFiList[i] = WiFi->WiFiList[j];
                WiFi->WiFiList[j] = temp;
            }

            if(strcasecmp(WiFi->WiFiList[i].status,WiFi->WiFiList[j].status)==0 &&
                WiFi->WiFiList[i].signalStrength < WiFi->WiFiList[j].signalStrength){
                temp = WiFi->WiFiList[i];
                WiFi->WiFiList[i] = WiFi->WiFiList[j];
                WiFi->WiFiList[j] = temp;
            }
        }
    }
}

void connectWifi(WiFiManagement *WiFi){
    char name[10];
    int password = 0;
    int isWiFiFound = 0;

    printf("Enter wifi name to connect: ");
    scanf("%s",name);

    for(int i=0; i<WiFiSize; i++){
        if(strcasecmp(WiFi->WiFiList[i].name,name)==0){

            isWiFiFound = 1;

            if(strcasecmp(WiFi->WiFiList[i].status,"Connected")==0){
                printf("Already Connected\n");
                return;
            }
            else if(strcasecmp(WiFi->WiFiList[i].status,"Saved")==0){
                printf("Connected\n");
                strcpy(WiFi->WiFiList[i].status,"Connected");
                strcpy(WiFi->WiFiList[0].status,"Saved");
                sortWifi(WiFi);
                return;
            }
            else if(strcasecmp(WiFi->WiFiList[i].status,"Available")==0){
                printf("Enter password: ");
                scanf("%d",&password);

                if(WiFi->WiFiList[i].password == password){
                    printf("Connected\n");
                    strcpy(WiFi->WiFiList[i].status,"Connected");
                    //strcpy(temp.status,"Saved");
                     strcpy(WiFi->WiFiList[0].status,"Saved");
                    sortWifi(WiFi);
                    return;
                }else{
                    printf("Entered password is incorrect, Try again\n");
                }
            }
        }
    }
    if(isWiFiFound == 0){
        printf("WiFi not found\n");
    }
}

// if(strcasecmp(wifi->wifiList[i].status,"Connected")==0){
//     strcpy(wifi->wifiList[i].status,"Saved");
// }

// if(strcasecmp(wifi->wifiList[i].status,"Saved")==0){
//     printf("Connected\n");
//     strcpy(wifi->wifiList[i].status,"Connected");
//     sortWifi(wifi);
// }

// if(strcasecmp(WiFi->wifiList[i].name,name)==0
//     && WiFi->wifiList[i].password == password){
//     printf("Connected\n");
//     strcpy(WiFi->wifiList[i].status,"Connected");
//     sortWifi(WiFi);
// }


void wifiMenu(WiFiManagement *WiFi){
    readData(WiFi);
    sortWifi(WiFi);

    enum{
        display_WiFi = 1,
        connect_WiFi = 2,
        Exit = 3,
    };

    int choice = 0;
    do{
        printf("\n1. Available WiFi\n2. Connect to WiFi\n3. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice){

        case display_WiFi:displayWifi(WiFi);
            break;

        case connect_WiFi:connectWifi(WiFi);
            break;

        case Exit:printf("Exiting...\n");
            break;

        default:printf("Inavlid choice\n");
        }
    }while(choice != 3);
}

