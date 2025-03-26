#ifndef MOBILELEFTSIDE_H
#define MOBILELEFTSIDE_H

#include "mobileSideDimension.h"
#include "simCardTool.h"
#include "volume.h"

struct MobileLeftSide{
    struct SideDimension leftDimension;
    struct Volume volume;
    struct SimCardTool sim;
};

#endif // MOBILELEFTSIDE_H
