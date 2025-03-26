#ifndef BOOK_H
#define BOOK_H

#include "backCover.h"
#include "bookDimension.h"
#include "contents.h"
#include "frontCover.h"

struct book{
    struct bookDimension bookDimension;
    struct frontCover bookfront;
    struct content content;
    struct backCover bookback;
};

#endif // BOOK_H
