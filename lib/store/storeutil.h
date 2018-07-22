#ifndef LIB_STORE_STOREUTIL_H
#define LIB_STORE_STOREUTIL_H

#include "store/store.h"

#include <stdbool.h>
#include <stdint.h>

int writeNumBitstoStore (store STORE, const uint64_t location,
                         const uint64_t wordStartBit, uint64_t number,
                         uint64_t length);
#endif
