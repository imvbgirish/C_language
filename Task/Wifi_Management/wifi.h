#ifndef WIFI_H
#define WIFI_H
#define MAX 10

typedef struct WiFi{
    char name[10];
    int signalStrength;
    char status[10];
    int password;
}WiFi;

// enum status{
//     Connected,
//     Active,
//     Available
// };

#endif // WIFI_H
