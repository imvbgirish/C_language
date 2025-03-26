#ifndef WIFIMANAGEMENT_H
#define WIFIMANAGEMENT_H

typedef struct wifiManagement{
    struct wifi *wifiList;
}wifiManagement;

void displayAllWifi(wifiManagement *wifi);
void sortWifi(wifiManagement *wifi);

#endif // WIFIMANAGEMENT_H
