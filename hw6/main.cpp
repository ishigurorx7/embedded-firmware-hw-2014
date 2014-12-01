#include <stdio.h>
#include "mbed.h"
#include "circularBuffer.h"
#include "boolMutex.h"

#define BUFF_SZ  64

// Ticker's interval periods
#define BUF_RW_FAST_INTERVAL  (0.010)  // 10 ms
#define BUF_RW_SLOW_INTERVAL  (0.015)  // 15 ms
#define BUF_USG_MTR_INTERVAL  (0.085)  // 85 ms

DigitalOut bufLed0(LED1);
DigitalOut bufLed1(LED2);
DigitalOut bufLed2(LED3);
DigitalOut bufLed3(LED4);

// Three tickers for three interrupt driven tasks.
Ticker buf_wrt_rate;
Ticker buf_rd_rate;
Ticker buf_mtr_rate;

CircularBuffer *buffp;
boolMutex      *mtxp;

void bufLed_onoff(int led0, int led1, int led2, int led3)
{
  bufLed0 = led0;
  bufLed1 = led1;
  bufLed2 = led2;
  bufLed3 = led3;
}

void test_bufLeds()
{
  bufLed_onoff(1, 1, 1, 1);
  wait(0.7);
  bufLed_onoff(1, 0, 1, 0);
  wait(0.7);
  bufLed_onoff(0, 1, 0, 1);
  wait(0.7);
  bufLed_onoff(1, 1, 1, 1);
  wait(0.7);
  bufLed_onoff(0, 0, 0, 0);
  wait(0.7);
}

void bufUsageMeter()
{
  static int cnt = 0; // for debugging
  INT8U usageCnt, size;
  int   usage;
  
  mtxp->lock();
  usageCnt = getCircularBufferUsage(buffp);
  mtxp->unlock();
  
  size     = buffp->size - 1;
  usage    = ((size - (size - usageCnt)) * 100)/size;
  
  if ((cnt++ % 6) == 0) {
    // for debugging
    pc.printf("METER: usage=%3d%c (rp=%3d wp=%3d sz=%3d)\n\r",
              usage, '%', buffp->readPointer, buffp->writePointer, buffp->size);
  }

  if (usage <= 1) {
    bufLed_onoff(0, 0, 0, 0);
  } else if (usage > 1 && usage <= 25) {
    bufLed_onoff(1, 0, 0, 0);
  } else if (usage > 25 && usage <= 50) {
    bufLed_onoff(1, 1, 0, 0);
  } else if (usage > 50 && usage <= 75) {
    bufLed_onoff(1, 1, 1, 0);
  } else if (usage > 75 && usage <= 100) {
    bufLed_onoff(1, 1, 1, 1);
  }
}

void myBufferEnque();
void myBufferDeque();

void myBufferEnque()
{
  static KeyType k = 0;

  mtxp->lock();
  
  if (!(CircularBufferEnque(buffp, k++))) {
    /**
     * update enque/deque's interval rates when 
     * buffer is full.
     */
    pc.printf("ENQUE: Buff full,  update ticker's inteval!\n\r");
    buf_wrt_rate.attach(&myBufferEnque, BUF_RW_SLOW_INTERVAL);
    buf_rd_rate.attach(&myBufferDeque, BUF_RW_FAST_INTERVAL);
  }
  
  mtxp->unlock();
}

void myBufferDeque()
{
  KeyType k;
  
  mtxp->lock();

  if (!(CircularBufferDeque(buffp, &k))) {
    /**
     * update enque/deque's interval rates when 
     * buffer is empty.
     */
    pc.printf("DEQUE: Buff mepty, update ticker's interval\n\r");
    buf_wrt_rate.attach(&myBufferEnque, BUF_RW_FAST_INTERVAL);
    buf_rd_rate.attach (&myBufferDeque, BUF_RW_SLOW_INTERVAL);
  }
  
  mtxp->unlock();
}

int main()
{
  boolMutex bmutex = boolMutex(false);
  mtxp = &bmutex;
  
  pc.printf("\n\r;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n\r");
  pc.printf(";;  START CIRCULAR BUFFER TEST  ;;\n\r");
  pc.printf(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n\r");     
  
  // diagnostic to mounted 4 LEDs first
  pc.printf("Diagnostic LEDs ...\n\r");
  test_bufLeds();

  // create & initialize the circular buffer
  pc.printf("Create & initialize circular buffer ...\n\r");
  if (!(CircularBufferInit(&buffp, BUFF_SZ))) {
    pc.printf("CircularBuffer initialization failed\n\r");
    return -1;
  }

  /**
   * Assignes three Tickers(interrupt driven task which is
   * called periodically in the specific rate.
   */
  pc.printf("Start Testing ...\n\r");
  pc.printf("Start Buffer Enque ticker, called every %6.2f ms ...\n\r",
             (BUF_RW_FAST_INTERVAL * 1000.0));
  pc.printf("Start Buffer Deque ticker, called every %6.2f ms ...\n\r",
             (BUF_RW_SLOW_INTERVAL * 1000.0));
  pc.printf("Start Usage  Meter ticker, called every %6.2f ms ...\n\r\n\r",
             (BUF_USG_MTR_INTERVAL * 1000.0));

  buf_wrt_rate.attach(&myBufferEnque, BUF_RW_FAST_INTERVAL);
  buf_rd_rate.attach (&myBufferDeque, BUF_RW_SLOW_INTERVAL);
  buf_mtr_rate.attach(&bufUsageMeter, BUF_USG_MTR_INTERVAL);

  while (1) {
    wait(0.2);
  }
  
  return 0;
}
