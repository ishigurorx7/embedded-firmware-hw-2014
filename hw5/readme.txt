Introduction
============

Since I don't have two mbed boards, I couldn't try the I pick program example 12.9(HTTPClinet program) instead of 12.{7,8} from the text book. Unfortunately I couldn't success to compile the "HTTPClient" program, because it uses very old class library named EthernetNetIf. Here is the actual error message on the online compiler:

Error: Undefined symbol mbed::Base::~Base() (referred from fwk-if-eth-EthernetNetIf.cpp.o).

 I replaced to better lib named "EthernetInterface", but now I got compilation failure from HTTPClient class library. I tried to fix it, but unfortunately undefined header file does not exist on the 'ARM mbed' database. So I gave up to work on the program example, and I pick following program example available on the mbed community:

http://developer.mbed.org/users/mbed714/code/TCPSocket_HelloWorldTest/

I did minor change on main.cpp to fit with my work environments:

1. Changed serial baud to 9600 'cos my PC is 8+ years old and using old USB.
2. Added extra '\r' char in the string to print.
3. Updated all used libraries; EthernetInterface, mbed, mbed-rtos.

Build was success, and was able to run the program.
Note that my LPC1768 was put on their application board which has RJ48 8 pints connector for Ethernet connection.

Please take a look following files:
1. mbedEthrConn.JPG
2. consoleOut.txt

Original instruction of the hw5 homework
========================================

4357 Embedded Firmware Essentials
Program Assignment #4 10 points
Read Chapter 12
12.4.3 Ethernet Commuication between mbeds.

Work with a classmate as a team for this Program Assignment.  
Connect two mbeds as show in Figure 12.8, 
and compile and download Program Example 12.7 and 12.8 into each mbed board.  

http://developer.mbed.org/forum/mbed/topic/1239/

E. Hook up to MBED
   - See MBED card or printed on the right side of MBED - Pin label is ABOVE THE PINS:
-------------------GREEN -------------RD + PIN on MBED ------

(To RJ45 Plug)---- GREEN/WHITE------- RD - PIN on MBED ------(To MBED RD +/- and TD+/- Pins)

------------------ ORANGE-------------TD + PIN on MBED-----

-------------------ORANGE/WHITE-------TD - PIN on MBED ----
