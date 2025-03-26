#ifndef WIFIMANAGEMENT_H
#define WIFIMANAGEMENT_H

typedef struct wifiManagement{
    struct wifi *wifiList;
    int wifiSize;
}wifiManagement;

//void displayAllWifi(wifiManagement *wifi);

void readData(wifiManagement *wifi);
void add(wifiManagement *wifi,char *name,int signalStrength,char *status,int password);
void displayWifi(wifiManagement *wifi);
void sortWifi(wifiManagement *wifi);

#endif // WIFIMANAGEMENT_H
