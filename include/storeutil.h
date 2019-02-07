#ifndef LIB_STORE_STOREUTIL_H
#define LIB_STORE_STOREUTIL_H

#include <stdbool.h>
#include <stdint.h>

#include "StoreLib/include/store.h"

int writeNumBitstoStore (store STORE, const uint64_t location,
                         const uint64_t wordStartBit, uint64_t number,
                         uint64_t length);
int writeMultiBitstoStore (store STORE, const uint64_t location,
                           const uint64_t wordStartBit, const bool bitArray[],
                           uint64_t length);
uint64_t readNumBitstoStore (store STORE, const uint64_t location,
                             const uint64_t wordStartBit,
                             uint64_t bitLength);
bool *readMultiBitsfromStore (const store STORE, const uint64_t location,
                              const uint64_t wordStartBit,
                              uint64_t numberofBits);
uint64_t readNumBitsfromStore(store STORE, const uint64_t location,
                              const uint64_t wordStartBit, uint64_t width);
int writeBytestoStore(store STORE, const uint64_t location,
                      const uint64_t wordStartBit, uint64_t number,
                      uint64_t byteLength, bool endianStyle);
uint64_t readBytesfromStore(const store STORE, const uint64_t location,
                            const uint64_t wordStartBit, const unsigned byteLength,
                            const bool endianStyle);
#endif
