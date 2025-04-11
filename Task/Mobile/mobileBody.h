#ifndef MOBILEBODY_H
#define MOBILEBODY_H

#include "mobileBodyDimension.h"
#include "mobileBottomSide.h"
#include "mobileLeftSide.h"
#include "mobileRightSide.h"
#include "mobileUpSide.h"

struct MobileBody{
    char type[10];
    char color[10];
    struct MobileBodyDimension dimension;
    struct MobileLeftSide left;
    struct MobileRightSide right;
    struct MobileBottomSide down;
    struct MobileUpSide up;
};

#endif // MOBILEBODY_H
