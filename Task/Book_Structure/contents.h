#ifndef CONTENTS_H
#define CONTENTS_H

#include "chapters.h"
#include "pages.h"

struct content{
    struct page pages[200];
    struct chapter chapters[10];
};

#endif // CONTENTS_H
