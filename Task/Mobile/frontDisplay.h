#ifndef FRONTDISPLAY_H
#define FRONTDISPLAY_H

#include "camera.h"
#include "displayDimension.h"

struct FrontDisplay{
    char type[10];
    struct DisplayDimension displaydimension;
    struct Camera cam;
};

#endif // FRONTDISPLAY_H
