#ifndef WIFIMANAGEMENT_H
#define WIFIMANAGEMENT_H

typedef struct WiFiManagement{
    struct WiFi *WiFiList;
    //int wifiSize;
}WiFiManagement;

//void displayAllWifi(WiFiManagement *wifi);

void readData(WiFiManagement *wifi);
void add(WiFiManagement *wifi,char *name,int signalStrength,char *status,int password);
void displayWifi(WiFiManagement *wifi);
void connectWifi(WiFiManagement *wifi);
void sortWifi(WiFiManagement *wifi);
void wifiMenu(WiFiManagement *wifi);


#endif // WIFIMANAGEMENT_H
