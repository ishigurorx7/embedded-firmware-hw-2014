#include "mbed.h"
#include "EthernetInterface.h"

#define RETRIES_ALLOWED 5

Serial pc2(USBTX, USBRX);
EthernetInterface eth;
DigitalOut led1(LED1);

int main()
{

    //pc2.baud(115200);
    printf("\n\rInitialising...\n\r");
    set_time(0);

    // use DHCP
    eth.init();

    // attempt DHCP and if timing out then try again
    while (eth.connect()) {
        printf("DHCP timeout\n\r");
    };

    // successful DHCP
    printf("IP Address is %s\n\r", eth.getIPAddress());

    // loop forever
    while (true) {

        // new non blocking socket
        TCPSocketConnection sock;
        sock.set_blocking(false);

        // toggle led to indicate activity
        led1 = !led1;

        // attempt socket connection and if failure then loop again
        if (sock.connect("mbed.org", 80)) {
            printf("Socket connect timeout\n");
            continue;
        }

        // send command
        printf("\n\rSending at %d seconds\n\r", time(NULL));
        char http_cmd[] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\n\n";
        sock.send_all(http_cmd, sizeof(http_cmd) - 1);

        // receive response
        int received = 0;
        int retries = 0;
        while (true) {

            int result;
            char buffer[256];

            result = sock.receive(buffer, sizeof(buffer) - 1);
            printf("Received: %d %d %d\n\r", result, received, retries);

            // if timeout or no data
            if (result <= 0) {

                // if data previously received then stop receiving
                if (received > 0)
                    break;

                // if incremented retries exceeded then no response and stop receiving
                if (++retries > RETRIES_ALLOWED) {
                    break;
                }

            } else {

                // zero terminate the buffer
                buffer[result] = '\0';
                
                // total size of data received
                received += result;
            }

        }

        // close socket at end of send and receive
        sock.close();
        
        // wait before repeating
        Thread::wait(1000);

    }
}