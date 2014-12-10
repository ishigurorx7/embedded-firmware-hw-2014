/*
 * This program demonstrates the LED flashing (external Green and Red LEDs) 
 * by manipulating microcontroller's internal register without using mbed library.
 * This program also provides the extra feature to calibrate accuracy of the software 
 * delay routine adjusted by Timer feature.
 *
 * Auther: Hisakazu Ishiguro
 * Date  : Tue Dec  9 16:31:36 PST 2014
 */

#include "mbed.h"  // for Timer and printf

void delay();
void delay_ms(unsigned int);
unsigned int get_delaycnt(unsigned int);

#define _FIO2DIR0  (*(volatile unsigned char *)(0x2009c040))
#define _FIO2PIN0  (*(volatile unsigned char *)(0x2009c054))

#define GREEN_LED_ON  (_FIO2PIN0 |=  0x01)
#define GREEN_LED_OFF (_FIO2PIN0 &= ~0x01)

#define RED_LED_ON    (_FIO2PIN0 |=  0x02)
#define RED_LED_OFF   (_FIO2PIN0 &= ~0x02)

int main()
{
  _FIO2DIR0 = 0xff;    // set port 2, lowest byte to output 
  int i = 0;
  int cnt_50ms;
  
  printf("\r\nCalibrate software delay timer ...\r\n");
  cnt_50ms = get_delaycnt(50);
  
  printf("\r\nGreen LED blinks every 100 mS\r\n");
  printf("Red   LED blinks every 1 Sec\r\n");
  printf("Start blink LEDs ...\r\n");
  
  while (1) {
    GREEN_LED_ON;
    if ((i % 10) == 9) RED_LED_ON;
    delay_ms(cnt_50ms);
    GREEN_LED_OFF;
    if ((i % 10) == 9) RED_LED_OFF;
    delay_ms(cnt_50ms);
    i++;
  }
}

float meas_delay(unsigned int max_cnt)
{
  Timer t; 
  int avg;
  volatile int j;
  float offset, tmeas;
  
  // Measure offset
  t.reset();
  t.start();
  for (j = 0; j < 0; j++) {
    j++;
    j--;
  }
  t.stop();
  offset = t.read_ms();
  
  // Calibrate meter
  tmeas = 0.0;
  
  for (avg = 0; avg < 5; avg++) {
    t.reset();
    t.start();
    for (j = 0; j < max_cnt; j++) {
      j++;
      j--;
    }
    t.stop();
    tmeas += (t.read_ms() - offset);
  }
  tmeas /= avg;
  return (tmeas);
}

unsigned int get_delaycnt(unsigned int ms)
{
#define CNT_10M (10000000)
  unsigned int delay_cnt;
  float tval_10m;
  
  // Calibrate delay: caliculate count value to make specified delay
  tval_10m = meas_delay(CNT_10M);
  delay_cnt = (unsigned int)((((float)(CNT_10M * ms))/tval_10m) + 0.5);
  return (delay_cnt);
}
  
void delay_ms(unsigned int ms)
{
  volatile int j;
  
  for (j = 0; j < ms; j++) {
    j--;
    j++;
  }
}
  

