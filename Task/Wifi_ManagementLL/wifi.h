#ifndef WIFI_H
#define WIFI_H

typedef struct wifi{
    char name[10];
    int signalStrength;
    char status[10];
    int password;
    struct wifi *next;
}wifi;

#endif // WIFI_H
