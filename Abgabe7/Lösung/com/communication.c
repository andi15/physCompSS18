// communication with serial device
//
// USAGE: comm <SERIAL_DEVICE>
// serial device is a path like /dev/ttysAC0 on Linux or /dev/cu.usbmodem1411 on MacOS or COM1 on Windows/Cygwin
//
// The program receives data via the serial connection from our board and prints it to the console once a complete
// line is received. Reading and buffering incoming data is realised with a LineBuffer datatype. The use of a
// line buffer to handling complete lines is motivated by the fact that is makes processing incoming data using
// sscanf() much easier.

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


#include "configureSerial.h"
#include "buffer.h"

int gRunning = 1; // global flag: set to 0 when program shall quit



// handle a line of input from serial (just print to stdout)
void processData(char *line) {

    // parse input to long
    printf("received data: ");
    char *end = line + strlen(line) -1;
    char **endPointer = &end;
    long uRelativ = strtol(line, endPointer, 2);
    printf("%ld (min: 2048 = 0 lumen, max: 0 => 1000 lumen)\n", uRelativ);

    printf("\n");
}



// arguments:
// argv[1] is the given path to the device location
int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "USAGE: %s <SERIAL_DEVICE>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // open serial device ("our board") for read-write and configure to 57600 8N1
    int serialDevice = open(argv[1], O_RDWR);

    if (serialDevice < 0) {
        perror("opening serial device failed");
        close(serialDevice);
        return EXIT_FAILURE;
    }
    serialDevice = configureSerial(serialDevice, 57600);

    if (serialDevice < 0) {
        perror("configuring serial device failed");
        close(serialDevice);
        return EXIT_FAILURE;
    }

	// set up file set for select() to wait for incoming data
	fd_set inputs;
    int fd_max = serialDevice+1;

    // init a buffer to hold complete line of data from board
    LineBuffer_p serialBuffer = initBuffer(&processData);

    printf("listening on %s, enter 'q' + enter to quit...\n", argv[1]); // FIXME: no console input -> no quitting the program :-(
	while (gRunning) {
		// set up the input set
		FD_ZERO(&inputs);
		FD_SET(serialDevice, &inputs);


        // set up a time-out (5 secs)
        struct timeval time;
        time.tv_sec = 5;  // 'full' seconds
        time.tv_usec = 0; // microseconds

        // wait/check for incoming data


		if(select(fd_max, &inputs, NULL, NULL, &time) > 0) {
            // we got data!
            // if serial device has sent something, read it!
            if (FD_ISSET(serialDevice, &inputs)) {
                lineBufferRead(serialBuffer, serialDevice);
            }
        } else {
            // time-out: no data for 5 seconds. Should we be worried?
            // no?
        }
    }
	  printf("Goodbye!\n");
    close(serialDevice);
    destroyBuffer(serialBuffer);
    return EXIT_SUCCESS;
}
