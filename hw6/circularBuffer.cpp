#include <stdio.h>
#include "mbed.h"
#include "circularBuffer.h"

Serial pc(USBTX, USBRX);

CircularBuffer* CircularBufferInit(CircularBuffer **pQue, int size) 
{
  int sz = size * sizeof(KeyType) + sizeof(CircularBuffer);

  if ((*pQue = (CircularBuffer *)malloc((size_t)sz)) != NULL) {
    pc.printf("Init CircularBuffer: keys[%d] total mem size=%d bytes\n\r", size, sz);

    /**
     * For indexing circular buffer, you should use AND (with mask)
     * operation rather than % operation (very slow). 
     * In order to do, buffer size MUST be power of 2.
     */
    if (!(size & (size - 1))) {
      (*pQue)->size         = size;
      (*pQue)->mask         = size - 1;
      (*pQue)->writePointer = 0;
      (*pQue)->readPointer  = 0;
    } else {
      pc.printf("Error Size:%d is not power of TWO!\n\r", size);
      return NULL;
    }
  } else {
    pc.printf("CircularBufferInit: malloc failed!\n\r");
  }
  return *pQue;
}
