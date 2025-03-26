#include <stdio.h>
#include "wifiManagement.h"

int main()
{
    wifiManagement wifi;
    wifi.wifiList = NULL;
    wifi.wifiSize = 0;

    readData(&wifi);
    displayWifi(&wifi);

    return 0;
}
