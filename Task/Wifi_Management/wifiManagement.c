#include "wifiManagement.h"
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

// void sortWifi(wifiManagement *wifi){
//     int i,j;
//     struct wifi temp;
//     for(i=0; i<num; i++){
//         for(j=i+1; j<num; j++){

//             if(strcasecmp(wifi->wifiList[i].status,"Connected")!=0
//                 && strcasecmp(wifi->wifiList[j].status,"Connected")==0){
//                 temp = wifi->wifiList[i];
//                 wifi->wifiList[i] = wifi->wifiList[j];
//                 wifi->wifiList[j] = temp;
//             }

//             if(strcasecmp(wifi->wifiList[i].status,"Available")==0
//                 && strcasecmp(wifi->wifiList[j].status,"Saved")==0){
//                 temp = wifi->wifiList[i];
//                 wifi->wifiList[i] = wifi->wifiList[j];
//                 wifi->wifiList[j] = temp;
//             }

//             if(strcasecmp(wifi->wifiList[i].status,wifi->wifiList[j].status)==0 &&
//                 wifi->wifiList[i].signalStrength < wifi->wifiList[j].signalStrength){
//                 temp = wifi->wifiList[i];
//                 wifi->wifiList[i] = wifi->wifiList[j];
//                 wifi->wifiList[j] = temp;
//             }
//         }
//     }
// }

int num = 0;
void readData(wifiManagement *wifi){
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
        add(wifi, name, signalStrength, status, password);
        num++;
    }
    fclose(fp);
}

void add(wifiManagement *wifi, char *name, int signalStrength, char *status, int password){
    if(wifi->wifiList == NULL){
        wifi->wifiList = (struct wifi *)malloc(MAX*sizeof(struct wifi));
        if(wifi->wifiList == NULL){
            printf("Memory Allocation failed");
            return;
        }
    }else{
        wifi->wifiList = (struct wifi *)realloc(wifi->wifiList,(num+1)* sizeof(struct wifi));
    }
    strcpy(wifi->wifiList[wifi->wifiSize].name,name);
    wifi->wifiList[wifi->wifiSize].signalStrength = signalStrength;
    strcpy(wifi->wifiList[wifi->wifiSize].status,status);
    wifi->wifiList[wifi->wifiSize].password = password;

    wifi->wifiSize++;
}

void displayWifi(wifiManagement *wifi){
    int i;
    if(wifi->wifiList == NULL){
        printf("No wifi data to display\n");
        return;
    }else{
        for(i=0; i<wifi->wifiSize; i++){
            printf("Name: %s\n", wifi->wifiList[i].name);
            printf("Signal Strength: %d\n", wifi->wifiList[i].signalStrength);
            printf("Status: %s\n", wifi->wifiList[i].status);
            printf("Password: %d\n", wifi->wifiList[i].password);
            printf("\n");
        }
    }
}
