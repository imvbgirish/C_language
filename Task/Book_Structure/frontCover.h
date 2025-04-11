#ifndef FRONTCOVER_H
#define FRONTCOVER_H

#include "bookCoverDimension.h"

struct frontCover{
    char title[20];
    char authorName[20];
    int bookEdition;
    int copiesSold;
    struct bookCoverDimension front;
};

#endif // FRONTCOVER_H
