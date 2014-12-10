
1. Finished reading Chapter 1,2,3 and 14.
2. Created program for flashing LEDs without using mbed lib.

Here is the detail:

This program demonstrates the LED flashing (external Green and Red LEDs) 
by manipulating microcontroller's internal register without using mbed library.
This program also provides the extra feature to calibrate accuracy of the software 
delay routine adjusted by Timer feature so that program makes to blink Green LED
in every 100mS and Red lED in every 1 Sec.

I verified that Red LED blinks every 1 Sec synchronized with the iPhone's timer tick.

Mbed h/w connections:
Here is the diagram of the LED(Green and Red) connection thru the pin 25 and 25.
Please see the attached photo.

                                         +----- LPC1768 MBED board -------------
                      Green LED          |
  +---[ 47OHM Reg ]----(|<++|)----<pin26>+ (port2, bit0)
  |                                      |
  |                    Red LED           |
  +---[ 47OHM Reg ]----(|<++|)----<pin25>+ (port2, bit1)
  |                                      |
  |                                      |
 ---
  v
 GND



~~~~~~~~~~~~~ Original README ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
4357 Embedded Firmware Essentials

- Program Assignment #3 10 points
- Objective
   1. Understand different between C++ and C APIs in mbed environment
   2. Control Digital IOs from a C or C++ code.

1. Read Chapter 1,2,3, and Chapter 14
   Program Example 14.1 Manipulating control registers to flash an LED
   Program Example 14.3 Combined digital input and output.
   Compile and download Program Example 14.1 and 14.3 into your mbed board.

2. Read https://github.com/Shengliang/mbed.git
