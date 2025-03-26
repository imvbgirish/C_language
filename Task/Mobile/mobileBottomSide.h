#ifndef MOBILEBOTTOMSIDE_H
#define MOBILEBOTTOMSIDE_H

#include "mobileSideDimension.h"
#include "speaker.h"
#include "chargingPort.h"

struct MobileBottomSide{
    struct SideDimension bottomDimension;
    struct ChargingPort port;
    struct Speaker speaker;
};

#endif // MOBILEBOTTOMSIDE_H
