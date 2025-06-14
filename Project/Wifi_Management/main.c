#include <stdio.h>
#include <stdlib.h>
#include "WiFiManagement.h"

int main()
{
    WiFiManagement wifi;
    wifi.WiFiList = NULL;
    //wifi.wifiSize = 0;

    wifiMenu(&wifi);

    return 0;
}
