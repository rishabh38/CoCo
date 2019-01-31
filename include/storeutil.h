#ifndef LIB_STORE_STOREUTIL_H
#define LIB_STORE_STOREUTIL_H

#include <stdbool.h>
#include <stdint.h>

#include "StoreLib/store.h"

int writeNumBitstoStore (store STORE, const uint64_t location,
                         const uint64_t wordStartBit, uint64_t number,
                         uint64_t length);
uint64_t readNumBitstoStore (store STORE, const uint64_t location,
                             const uint64_t wordStartBit,
                             uint64_t bitLength);
uint64_t readNumBitsfromStore(store STORE, const uint64_t location,
                              const uint64_t wordStartBit, uint64_t width);
int writeBytestoStore(store STORE, const uint64_t location,
                      const uint64_t wordStartBit, uint64_t number,
                      uint64_t byteLength, bool endianStyle);
uint64_t readBytestoStore(const store STORE, const uint64_t location,
                          const uint64_t wordStartBit, uint64_t byteLength,
                          const bool endianStyle);
#endif
