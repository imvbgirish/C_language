#ifndef MOBILE_H
#define MOBILE_H

#include "frontDisplay.h"
#include "mobileBody.h"
#include "mobileDimension.h"

struct Mobile{
    char brand[10];
    char model[10];
    int price;

    struct MobileDimension mobileSize;
    struct FrontDisplay display;
    struct MobileBody body;
};

#endif // MOBILE_H
