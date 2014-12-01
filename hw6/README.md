CircularBuffer
==============

1 Introduction
==============

This repository provides simple circular buffer routines and their test program for mbed LPC1768 board.
The circular buffer and their utility routines are defined in cirucluarBuffer.h header, and their initialization routine is defined in circularBuffer.cpp file. This program supports en-queue and de-queue features only, and these routine returns un-success value when queue is full detected by the en-queue routine or queue is empty by the de-queue routine. Those routine are no secure when it's accessed by the multiple tasks such as ISR, but there are wrapper functions of en-queue and de-queue defined in main.cpp named 'myBufferEnque() and myBufferDeque(), those routine, uses mutex operation (mutual exclusive operation) to protect critical regions before calling en-queue and de-queue functions defined on the 'circularBuffer.h'.

2 Test method (three tickers)
=============================

  The test program consists of following three tickers (interrupt driven tasks that are periodically called by timer tick):

* myBufferEnque:  En-queueing single element from buffer every 10 ms
* myBufferDeque:  De-queueing single element from buffer every 15 ms
* bufUsageMeter:   Display LED(s) to shows buffer usage every 85 ms

3 Test sequence
===============

Here is the sequence of circular buffer test:

* Diagnostics mbed (LPC1768)'s mounted 4 LEDs by displaying few different patterns. It takes 2-4 seconds.
* Kick off 3 tickers mentioned above, and operator can see the state by seeing 4 LED's and console output if you setup the USB-console setups.
* Whenever en-queue or de-queue encounters their non-programmable conditions; buffer full or empty, their ticker's interval time amount is switched between myBufferEnque and myBufferDeque so that program can continue be executed.

4 Notes
=======

This program was tested on LPC1768 using binary created by the mbed online compiler, and not tested with binary from GNU GCC tool chain yet.
The en-queue and de-queue core routines should be organized as library so that other application can use it.

5 Console output
================
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  START CIRCULAR BUFFER TEST  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Diagnostic LEDs ...
Create & initialize circular buffer ...
Init CircularBuffer: keys[64] total mem size=68 bytes
Start Testing ...
Start Buffer Enque ticker, called every  10.00 ms ...
Start Buffer Deque ticker, called every  15.00 ms ...
Start Usage  Meter ticker, called every  85.00 ms ...

METER: usage=  4% (rp=  5 wp=  8 sz= 64)
METER: usage= 31% (rp= 39 wp= 59 sz= 64)
METER: usage= 58% (rp=  9 wp= 46 sz= 64)
METER: usage= 85% (rp= 43 wp= 33 sz= 64)
ENQUE: Buff full,  update ticker's inteval!
METER: usage= 88% (rp= 17 wp=  9 sz= 64)
METER: usage= 61% (rp=  4 wp= 43 sz= 64)
METER: usage= 34% (rp= 55 wp= 13 sz= 64)
METER: usage=  7% (rp= 42 wp= 47 sz= 64)
DEQUE: Buff mepty, update ticker's interval
METER: usage= 15% (rp= 14 wp= 24 sz= 64)
METER: usage= 42% (rp= 48 wp= 11 sz= 64)
METER: usage= 69% (rp= 18 wp= 62 sz= 64)
METER: usage= 96% (rp= 52 wp= 49 sz= 64)
ENQUE: Buff full,  update ticker's inteval!
METER: usage= 77% (rp= 33 wp= 18 sz= 64)
METER: usage= 50% (rp= 20 wp= 52 sz= 64)
METER: usage= 23% (rp=  7 wp= 22 sz= 64)
DEQUE: Buff mepty, update ticker's interval
METER: usage=  1% (rp= 53 wp= 54 sz= 64)
METER: usage= 28% (rp= 23 wp= 41 sz= 64)
METER: usage= 55% (rp= 57 wp= 28 sz= 64)
METER: usage= 82% (rp= 27 wp= 15 sz= 64)

