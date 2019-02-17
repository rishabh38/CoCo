#include "StoreLib/include/store.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


#define ERROR_MESSAGE(location,reason,returnValue) ("\nin" location ":\n" reason ", returning " returnValue "\n")
#define INITIALIZE_ERROR_MESSAGE(reason) ERROR_MESSAGE("initializeStore", reason, "uninitiated store")
#define WRITE_ERROR_MESSAGE(reason) ERROR_MESSAGE("writeBittoStore", reason, "-1")
#define READ_ERROR_MESSAGE(reason) ERROR_MESSAGE("readBitfromStore", reason, "-1")




/* bool **createMatrix(uint64_t totalRows, uint64_t totalColumns):
 * Takes total number of rows and columns, and returns a pointer
 * pointing to two dimensional bool array. Returns NULL if unable to
 * allocate store to any pointer.
 * WARNING: giving large values can make program run like a snail, and take
 * for integer to unsigned integer conversions.
 */
bool **createMatrix(uint64_t totalRows, uint64_t totalColumns){
  bool **matrix = (bool**)malloc(sizeof(bool*) * totalRows);
  if(matrix == NULL){
    printf(ERROR_MESSAGE("createMatrix",
           "unable to allocate space to data matrix", "NULL"));
    return NULL;
  }

  bool NULLcheck = false;
  for (uint64_t index = 0; index < totalRows; index++){
    matrix[index] = (bool*)malloc(sizeof(bool) * totalColumns);
    if(matrix[index] == NULL){
      NULLcheck = true;
      break;
    }
  }
  if (NULLcheck == true){
    printf(ERROR_MESSAGE("createMatrix",
           "unable to allocate space to data matrix", "NULL"));
    return NULL;
  }

  return matrix;
}

/* store initializeStore(uint64_t wordSize, uint64_t totalLocations):
 * Takes wordSize and total no. of locations of store.
 * Returns a store object and set 'set' bit to 1.
 * Returns uninitiated store object i.e. all value = 0/NULL, if unable
 * to allocate space for the matrix pointer.
 * WARNING: giving large values can make program run like a snail, and take
 * care for integer to unsigned integer conversions.
 */
store initializeStore(const uint64_t totalLocations,
                        const uint64_t wordSize){
  store STORE;

  STORE.matrix = createMatrix(totalLocations, wordSize);
  if (STORE.matrix == NULL){
    printf(INITIALIZE_ERROR_MESSAGE("unable to allocate space to data matrix"));
    return STORE;
  }

  STORE.wordSize = wordSize;
  STORE.totalLocations = totalLocations;
  STORE.set = true;

  return STORE;
}

/* bool writeBittoStore (store givenStore, const uint64_t location,
                          const uint64_t bitinWord, const bool value):
 * Takes a store object, location in the matrix with row no. = location,
 * column no. = bitinWord, and the value to be written = value.
 * Returns 0 if written successfully, else returns -1.
 * WARNING: Take care of the integer typecasting while
 * calling the function.
 */
int writeBittoStore (store givenStore, const uint64_t location,
                      const uint64_t bitinWord, const bool value){

  if (!givenStore.set){
    printf(WRITE_ERROR_MESSAGE("givenStore isn't formally initialized yet"));
    return -1;
  }

  if (location >= givenStore.totalLocations){
    printf(WRITE_ERROR_MESSAGE("requested 'location' %" PRIu64 " out of bound"),
           location);
    return -1;
  }

  if (bitinWord >= givenStore.wordSize){
    printf(WRITE_ERROR_MESSAGE("requested 'bit in Word' %" PRIu64 " out of bound"),
           bitinWord);
    return -1;
  }

  givenStore.matrix[location][bitinWord] = value;

  return 0;
}

/* int readBitfromStore (const store givenStore, const uint64_t location,
                          const uint64_t bitinWord):
 * takes store type object, location number, and bit number in word.
 * Returns value stored at that store location if successful,
 * else returns -1.
 */
int readBitfromStore (const store givenStore, const uint64_t location,
                       const uint64_t bitinWord){

  if(!givenStore.set){
    printf(READ_ERROR_MESSAGE("givenStore is not formally initialized yet"));
    return -1;
  }

  if (location >= givenStore.totalLocations){
    printf(READ_ERROR_MESSAGE("given 'location' %" PRIu64 " is out of bound"),
              location);
    return -1;
  }

  if (bitinWord >= givenStore.wordSize){
    printf(READ_ERROR_MESSAGE("given 'bit in word' %" PRIu64 " is out of bound"),
           bitinWord);
    return -1;
  }

  bool value = givenStore.matrix[location][bitinWord];

  return value;
}


/* size_t sizeofStore(store STORE):
 * Takes store object.
 * Returns size of the matrix stored in the object, returns 0
 * if the object is not initialized.
 */
size_t sizeofStore(store STORE){
  if (!STORE.set){
    printf(ERROR_MESSAGE("sizeofStore", "store is not formally initialized",\
                         "0"));
    return 0;
  }

  size_t storeSize = STORE.totalLocations * STORE.wordSize;

  return storeSize;
}
