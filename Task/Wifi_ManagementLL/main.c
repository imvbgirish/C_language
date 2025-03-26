#include "wifiManagement.h"
#include <stdio.h>

int main()
{
    struct wifiManagement wifi;
    wifi.wifiList = NULL;
    displayAllWifi(&wifi);

    sortWifi(&wifi);
    return 0;
}
