#ifndef MOBILEDIMENSION_H
#define MOBILEDIMENSION_H

#include "displayDimension.h"
#include "mobileBodyDimension.h"

struct MobileDimension{
    struct MobileBodyDimension body;
    struct DisplayDimension display;
};

#endif // MOBILEDIMENSION_H
