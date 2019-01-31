#ifndef LIB_STORE_STORE_H
#define LIB_STORE_STORE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* struct store:
 * Data structure to hold data of the store
 * and info about the status of the particular
 * store. Data for matrix should be handled
 * separately as structure in c is non dynamic.
 */
typedef struct{
  int wordSize;
  int totalLocations;
  bool set;
  bool **matrix;
}store;

store initializeStore(const uint64_t totalLocations,
                      const uint64_t wordSize);
int writeBittoStore(store givenStore, const uint64_t location,
                    const uint64_t bitinWord, const bool value);
int readBitfromStore(const store givenStore, const uint64_t location,
                     const uint64_t bitinWord);
size_t sizeofStore(store givenStore);
#endif
