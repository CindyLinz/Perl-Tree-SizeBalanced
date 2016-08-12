// vim: filetype=xs

#ifndef TREE_COMMON__H
#define TREE_COMMON__H

#include "EXTERN.h"
#include "perl.h"

#define SEG_SIZE (64)

typedef IV (*cmp_t)(SV * a, SV * b);

#endif
