#include "wifiManagement.h"
#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayAllWifi(wifiManagement *wifi){
    FILE *fp;
    fp = fopen("wifidata.txt","r");

    if(fp == NULL){
        printf("cannot open file");
        exit(0);
    }
    struct wifi *wifidata;

    while(1){
        wifidata = (struct wifi *)malloc(sizeof(struct wifi));
        if(wifidata == NULL){
            printf("Memory Allocation failed");
            return;
        }
        if(fscanf(fp,"%s %d %s %d",wifidata->name, &wifidata->signalStrength, wifidata->status, &wifidata->password)==EOF){
            free(wifidata);
            break;
        }
        wifidata->next = NULL;

        if(wifi->wifiList == NULL){
            wifi->wifiList = wifidata;
        }else{
            struct wifi *temp = wifi->wifiList;
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = wifidata;
        }
    }
    fclose(fp);
}

void sortWifi(wifiManagement *wifi){

    struct wifi *temp = wifi->wifiList, *head = wifi->wifiList,*nextnode = NULL,*prevnode = NULL;
    if(temp == NULL){
        printf("No data to display");
        return;
    }
    while (temp != NULL) {
        if (strcasecmp(temp->status, "Connected")==0) {
            if (prevnode != NULL) {
                prevnode->next = temp->next;
            }
            nextnode = temp->next;
            temp->next = head;
            head = temp;
            temp = nextnode;
        } else {
            prevnode = temp;
            temp = temp->next;
        }
    }
    temp = head;

    while(temp != NULL){
        printf("Name: %s\n", temp->name);
        printf("Signal Strength: %d\n", temp->signalStrength);
        printf("Status: %s\n", temp->status);
        printf("Password: %d\n", temp->password);
        printf("\n");
        temp = temp->next;
    }
}
