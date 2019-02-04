#include "StoreLib/include/storeutil.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "StoreLib/include/store.h"

#define BIGENDIAN 0
#define LITTLEENDIAN 1

#define ERROR_MESSAGE(location,reason,reaction) ("\nin " location ":\n"\
                                                 reason ", " reaction "\n")
#define STORE_ERROR(reason) ERROR_MESSAGE("checkStore",\
                                          reason,"returning -1")



/* int checkStore(const store STORE, const uint64_t location,
               const uint64_t wordStartBit):
 * Takes store object, location in store, bit in Word in store, and
 * checks whether
 *  -store object is initialized,
 *  -location is in bound for store object,
 *  -word bit is in bound for store object.
 * if any of the above is false, RETURNS -1, else RETURNS 0
 */
int checkStore(const store STORE, const uint64_t location,
               const uint64_t wordBit){
  if (!STORE.set){
    printf(STORE_ERROR("STORE not initialized"));
    return -1;
  }
  if (location >= STORE.totalLocations){
    printf(STORE_ERROR("Requested location %" PRIu64 " out of bound"), location);
    return -1;
  }
  if (wordBit >= STORE.wordSize){
    printf(STORE_ERROR("Requested 'bit in word'%" PRIu64 " out of bound"),
           wordBit);
    return -1;
  }

  return 0;
}



/* writeMultiBitstoStore (store STORE, const uint64_t location,
                          const uint64_t wordStartBit, const bool bitArray[],
                          uint_64 length):
 * Takes store object, location in store object, bit location in a word of
 * store, bool array, length of the array.
 * Truncates the bool Array if (length + wordStartBit) > wordSize of store
 * object.
 * Returns -1 if STORE object is not initialized, location is greater than
 * equal to totalLocations of STORE object, wordStartBit is greater than
 * equal to STORE's wordSize.
 * Returns 0 if write operation successful , even after truncating the array.
 * WARNING: Take care for keeping bitArray's actual length greater than
 * equal to 'uint64_t length' provided as parameter in the function.
 */
int writeMultiBitstoStore (store STORE, const uint64_t location,
                           const uint64_t wordStartBit, const bool bitArray[],
                           uint64_t length){


  if (checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("writeMultiBitstoStore", "checkStore returned -1",
                         "returning -1"));
    return -1;
  }

  uint64_t appliedLength = length;
  /* STORE.wordSize - wordStartBit
   * can't overflow as STORE.wordSize > wordStartBit,
   * thanks to the checking of the same previously.*/
  if((STORE.wordSize - wordStartBit) < length){
    printf(ERROR_MESSAGE("writeMultiBitstoStore",
                         "WARNING: bitArray length is too big to fit in word",
                         "truncating the bitArray"));
    appliedLength = STORE.wordSize - wordStartBit;
  }

  /* bitArray is stored in store such that, when store word is converted
   * into decimal number, lowest address word bit stores most significant bit,
   * whereas highest address word bit stores least significant bit.
   * Loop is designed with that in mind.*/
  for (uint64_t index = 0; index < appliedLength; index++){
    writeBittoStore(STORE, location, wordStartBit + index,
                    bitArray[appliedLength - index - 1]);
  }

  return 0;
}



/* bool *readMultiBitsfromStore (const store STORE, const uint64_t location,
                              const uint64_t wordStartBit,
                              uint64_t numberofBits):
 * Takes store object, location in store, bit location in word of store,
 * length - number of bits to read from store.
 * Reads multiple bits from a word of the store and returns array of bool
 * storing the bits of word from the store.
 * Retuns NULL if checkStore fails.
 */
bool *readMultiBitsfromStore (const store STORE, const uint64_t location,
                              const uint64_t wordStartBit,
                              uint64_t numberofBits){

  if(checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("readMultiBitsfromStore","checkStore returned -1",
                         "returning NULL"));
    return NULL;
  }

  uint64_t avlNumberofBits = numberofBits;
  /* STORE.wordSize - wordStartBit can't overflow as
   * STORE.wordSize > wordStartBit, thanks to the checking of the same
   * previously.*/
  if((STORE.wordSize - wordStartBit) < numberofBits){
    printf(ERROR_MESSAGE("readMultiBitsfromStore",
                         "WARNING: Requested number of bits greater than total"
                         " bits in Word after wordStartBit",
                         "storing only available bits in bitArray"));
    avlNumberofBits = STORE.wordSize - wordStartBit;
  }

  /* bitArray stores value(bits) such that, when bit array is converted
   * into decimal number, highest array index stores most significant bit,
   * whereas lowest array index stores least significant bit.
   * Loop is designed with that in mind.*/
  bool *bitArray = (bool*)malloc(sizeof(bool)*avlNumberofBits);
  for (uint64_t index = 0; index < avlNumberofBits; index++){
    bitArray[avlNumberofBits - index - 1] =
     readBitfromStore(STORE, location, wordStartBit + index);
  }

  return bitArray;
}



/* bool *numbertoBitString(uint64_t number, uint64_t length):
 * Takes unsigned number and length of bool array to store,
 * converts number into array of binary digits stored in
 * bool array. Converts such that most significant bit is the
 * highest index element, and least significant bit is the lowest
 * index element.
 * Returns this bool array.
 * Exceptions? not found yet.. help me out if you find one.
 */
bool *numbertoBitString(uint64_t number, uint64_t length){
  uint64_t actionNumber = number;
  bool *bitString = (bool*)malloc(sizeof(bool)*length);
  for (uint64_t i = 0; i < length; i++){
      bitString[i] = actionNumber % 2;
      actionNumber /= 2;
  }

  return bitString;
}



/* int writeNumBitstoStore (store STORE, const uint64_t location,
                         const uint64_t wordStartBit, uint64_t number,
                         uint64_t length):
 * Same as writeMultiBitstoStore, but takes number as input to store
 * in the word. Returns -1 if checkStore fails, else 0.
 */
int writeNumBitstoStore (store STORE, const uint64_t location,
                         const uint64_t wordStartBit, uint64_t number,
                         uint64_t length){

  if(checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("writeNumBitstoStore", "checkStore returned -1",
                         "returning -1"));
    return -1;
  }

  uint64_t availableBits = STORE.wordSize - wordStartBit;
  uint64_t bitstoWrite = (length <= availableBits)?length: availableBits;
  if (bitstoWrite < 64 && number >= pow(2, bitstoWrite)){
      printf(ERROR_MESSAGE("writeNumBitstoStore",
                           "WARNING: given number is bigger than"
                           " requested bit length",
                           "Truncating the number!"));
      number %= (uint64_t)pow(2, bitstoWrite);
  }

  bool *bitArray = numbertoBitString(number, bitstoWrite);
  writeMultiBitstoStore (STORE, location, wordStartBit, bitArray,
                         bitstoWrite);
  return 0;
}



/* uint64_t bitStringtoNumber (bool bitString[], uint16_t length):
 * takes array of bool and length as number of bits to retrieve
 * from array. Converts bit array into number and returns it.
 */
uint64_t bitStringtoNumber (bool bitString[], uint64_t bitLength){
  uint64_t number = 0;
  bitLength = bitLength*!(bitLength/64) + 64*(bool)(bitLength/64);

  for (uint16_t index = 0; index < bitLength; index++)
    number = number*2 + bitString[bitLength - index - 1];

  return number;
}



/* uint64_t readNumBitsfromStore(store STORE, const uint64_t location,
                                 const uint64_t wordStartBit,
                                 uint64_t bitWidth):
 * Reads store width in the form of number from wordStartBit to
 * wordStartBit + bitWidth - 1, with lowest address as most significant bit
 * and highest address as least significant bit.
 *
 * Takes store object, location in store, bit location in word of store,
 * and bit width of the number to be read, if bitWidth + wordStartBit
 * exceeds wordSize, then reads bits till wordSize.
 * Returns the resulting number.
 * throws an error and returns 0 when checkStore returns -1.
 */
uint64_t readNumBitsfromStore(store STORE, const uint64_t location,
                              const uint64_t wordStartBit, uint64_t bitWidth){
  if (checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("readNumBitsfromStore","checkStore returned -1",
                          "Returning 0"));
    return 0;
  }

  uint64_t availableBits = STORE.wordSize - wordStartBit;
  uint64_t bitstoRead = bitWidth;

  if(availableBits < bitWidth){
    printf(ERROR_MESSAGE("readNumBitstoStore",
                         "available bits in word after wordStartBit less"\
                         " than requested number of bits",
                         "returning number of smaller bit "\
                         "size than requested"));
    bitstoRead = availableBits;
  }

  bool *bitArray = readMultiBitsfromStore(STORE, location,
                                          wordStartBit, bitstoRead);
  uint64_t number = bitStringtoNumber(bitArray, bitstoRead);

  return number;
}



/* uint64_t giveBytestoUse(const uint64_t wordSize, const uint64_t wordStartBit,
                           const uint64_t byteLength):
 * gives how many bytes between wordStartBit and wordStartBit + 8*byteLength
 * in word are available to be used for reading/writing.
 * Takes total size of word, starting bit in word, total requested byte length.
 * Returns usable bytes between wordStartBit and wordStartBit + 8*byteLength.
 * Made to take care of the overflow possibility in word.
 */
uint64_t giveBytestoUse (const uint64_t wordSize, const uint64_t wordStartBit,
                         const uint64_t byteLength){
  uint64_t availableBytes = (wordSize - wordStartBit) / 8;
  uint64_t bytestoUse = byteLength;
  if (availableBytes < byteLength){
    printf(ERROR_MESSAGE("giveBytestoUse",
                         "WARNING: available byte length is smaller "\
                         "than requested byte length ",
                         "using available bytes only."));
    bytestoUse = availableBytes;
  }

  return bytestoUse;
}



/* uint64_t invertEndian (uint64_t number, unsigned byteLength):
 * Invert the endian format fo given number.
 * Takes number and number of bytes for the number to store.
 * Returns the inverted number.
 * Truncates the higher significant bits when number is large to
 * store in byteLength.
 */
uint64_t invertEndian(uint64_t number, unsigned byteLength){
  //cannot convert number greater than 8 bytes
  byteLength = byteLength*!(byteLength/8) + 8*(bool)(byteLength/8);

  uint64_t result = 0;

  for (unsigned index = 0; index < byteLength; index++){
    result = result*256 + number % 256;
    number /= 256;
  }

  return result;
}



/* int writeBytestoStore (store STORE, uint64_t location,
                          const uint64_t wordStartBit, uint64_t number,
                          const bool endianStyle, uint64_t byteLength):
 * Stores data in store in the form of bytes according to the endian format.
 * Takes store object, location in the store matrix, starting bit in the
 * word, total number of bytes, endian style and number as data to store.
 * Returns the success signal, 0 if successful, else 1.
 * Fails if checkStore returns -1
 */
int writeBytestoStore (store STORE, uint64_t location,
                       const uint64_t wordStartBit, uint64_t number,
                       uint64_t byteLength, const bool endianStyle){
  if (checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("writeBytestoStore",
                         "checkStore returned -1",
                         "Returning -1"));
    return -1;
  }

  uint64_t bytestoWrite = giveBytestoUse(STORE.wordSize, wordStartBit,
                                         byteLength);

  if (endianStyle == LITTLEENDIAN){
    number = invertEndian(number, bytestoWrite);
  }

  if(writeNumBitstoStore(STORE, location, wordStartBit, number,
                         bytestoWrite * 8)){
    printf(ERROR_MESSAGE("writeBytestoStore",
                         "writeNumBitstoStore returned -1",
                         "returning -1"));
    return -1;
  }

  return 0;
}



/* uint64_t readBytesfromStore (const store STORE, const uint64_t location,
                                const uint64_t wordStartBit,
                                const bool endianStyle,
                                const unsigned byteLength):
 * Reads data from word of store in the form of bytes.
 * Takes store object, location in store, wordStartBit, endian style which
 * is used to store in word, and the number of bytes to read.
 * Returns number in big endian form.
 * If something goes wrong, retuns 0.
 */
uint64_t readBytesfromStore (const store STORE, const uint64_t location,
                             const uint64_t wordStartBit,
                             const unsigned byteLength,
                             const bool endianStyle){
  if (checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("readBytesfromStore", "checkStore return -1",
                         "returning 0"));
    return 0;
  }

  uint64_t bytestoRead = giveBytestoUse(STORE.wordSize, wordStartBit,
                                        byteLength);

  uint64_t number = readNumBitsfromStore(STORE, location, wordStartBit,
                                         bytestoRead * 8);

  if (endianStyle == LITTLEENDIAN){
    number = invertEndian(number, bytestoRead);
  }

  return number;
}
