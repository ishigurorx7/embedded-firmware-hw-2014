#ifndef __CIRCULAR_BUFFER
#define __CIRCULAR_BUFFER

extern Serial pc;

typedef unsigned char  INT8U;
typedef unsigned short INT16U;

typedef INT8U KeyType;

typedef struct
{
  INT8U writePointer;    /**< write pointer */
  INT8U readPointer;     /**< read pointer  */
  INT8U size;            /**< size of circular buffer */ 
  INT8U mask;            /**< bit Mask of circular buffer size */
  KeyType keys[0];       /**< Element of circular buffer */
} CircularBuffer;

inline int CircularBufferIsFull(CircularBuffer* que)
{
  /**
   * Last element in buffer for write operation is not used so that
   * it's much simpler to implement this function.
   */
  if (((que->writePointer + 1) & que->mask) == que->readPointer)
    return 1;
  return 0;
}

inline int CircularBufferIsEmpty(CircularBuffer* que)
{
  if (que->readPointer == que->writePointer)
    return 1;
  return 0;
}

inline int getCircularBufferUsage(CircularBuffer* que)
{
  INT8U wp, rp, usage;

  if (CircularBufferIsEmpty(que)) {
    return 0;
  }

  if (CircularBufferIsFull(que)) {
    return (que->mask);
  }

  wp = que->writePointer;
  rp = que->readPointer;
  
  if (wp > rp)
    usage = wp - rp;
  else usage = que->size - rp + wp;
  
  if (usage >= que->size) {
    pc.printf("ERR, getCircularBufferUsage: got invalid usage(=%d)\n\r",
               usage);
    return -1;
  }
  return usage;
}

/**
 * CircularBufferEnque
 * Return : 1 for success, otherwise 0.
 */
inline int CircularBufferEnque(CircularBuffer* que, KeyType k)
{
  if (!CircularBufferIsFull(que)) {
    que->keys[que->writePointer] = k;
    que->writePointer++;
    que->writePointer &= que->mask;
    return 1;
  } 
  return 0;
}

/**
 * CircularBufferDeque
 * Return : 1 for success, otherwise 0.
 */
inline int CircularBufferDeque(CircularBuffer* que, KeyType* pK)
{           
  if (!CircularBufferIsEmpty(que)) {
    *pK = que->keys[que->readPointer];
    que->readPointer++;
    que->readPointer &= que->mask;
    return 1;
  }
  return 0;
}

inline int CircularBufferPrint(CircularBuffer* que)
{
  INT8U i, j;

  if (CircularBufferIsEmpty(que)) {
    pc.printf("Buffer is empty!\n\r");
    return 0;
  }

  j = 0;
  i = que->readPointer;

  while (i != que->writePointer) {
    pc.printf("Key[%d] = %d\n\r", i, que->keys[i]);
    j++;
    i++;
    i &= que->mask;
  }
  pc.printf("%d elems in buffer.\n\r", j);
  return j;
}

inline int CircularBufferPrint2(CircularBuffer* que)
{
  INT8U i, j;
  int usage;

  if (CircularBufferIsEmpty(que)) {
    pc.printf("Buffer is empty!\n\r");
    return 0;
  }

  j = 0;
  i = que->readPointer;
  usage = getCircularBufferUsage(que);

  pc.printf("BUFState: sz=%d usg=%d wp=%d rp=%d data={",
        que->size, usage, que->writePointer, que->readPointer);

  while (i != que->writePointer) {
    pc.printf("%d,", que->keys[i]);
    j++;
    i++;
    i &= que->mask;
  }
  pc.printf("\n\r");
  return j;
}

extern CircularBuffer* CircularBufferInit(CircularBuffer **pQue, int size);
#endif
