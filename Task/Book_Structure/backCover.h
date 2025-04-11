#ifndef BACKCOVER_H
#define BACKCOVER_H

#include "bookCoverDimension.h"

struct backCover{
    char aboutAuthor[100];
    char review[20];
    char publisherName[20];
    struct bookCoverDimension back;
};

#endif // BACKCOVER_H
