#include "Serial.h"
#include "../Global.h"

static Serial* serial; /// the serial object itself

Serial::Serial() {
	struct termios termOptions;
	isStopped = false;

	//O_RDWR: open for reading and writing.
	sd = open(SERIAL_INTERFACE_1, O_RDWR | O_NOCTTY );
	if (sd < 0) {
		std::printf("opening %s failed\n", SERIAL_INTERFACE_1);
		std::exit(EXIT_FAILURE);

	} else {
		std::printf("opening %s SUCCESSFUL\n", SERIAL_INTERFACE_1);
	}

	tcflush(sd, TCIOFLUSH); // flushes all data received but not read and all data written but not transmitted.
	sleep(1);

	tcgetattr(sd, &termOptions); // get the current terminal control settings.
	cfsetispeed(&termOptions, B19200); // set the input baud rate in termOptions.
	cfsetospeed(&termOptions, B19200); // Set the output baud rate in termOptions.

	// configure terminal control settings
	termOptions.c_cflag &= ~CSIZE; // clear number of data bits
	termOptions.c_cflag &= ~CSTOPB; // 2 stop bits
	termOptions.c_cflag &= ~PARENB; // no parity bit
	termOptions.c_cflag |= CS8; // 8 Data bits
	termOptions.c_cflag |= CREAD; // enable receiving characters
	termOptions.c_cflag |= CLOCAL; // local connection, ignore modem status lines.

	tcsetattr(sd, TCSANOW, &termOptions); // change the terminal control settings immediately.
}

Serial::~Serial() {
	if (close(sd) < 0) {
		std::printf("error closing %s fileDescriptor\n", SERIAL_INTERFACE_1);
	}
}

/// returns the only running instance of Gate
Serial* Serial::getInstance() {
	if (!init_HW_Done()) {
		init_HW();
	}

	if (!serial) {
		serial = new Serial();
	}

	return serial;
}

int Serial::sendMessage(unsigned char message[]) {
	//message = "INIT \r";
	//int n_written = 0;
	//n_written += write( USB, &cmd[n_written], 1 );
	int n = 0;
	n = write(sd, message, sizeof(message) - 1);
	if (n < 0) {
		cout << "Error sending: " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	} else if (n == 0) {
		cout << "send nothing!" << endl;
	}

	return n;
}

int Serial::readMessage(char* buf) {
	int n = 0;
	n = readcond(sd, buf, sizeof(buf) - 1, 1, 0, 10);
	if (n < 0) {
		cout << "Error reading: " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	} else if (errno == EAGAIN || n == 0) {
		cout << "Read nothing! or EAGAIN" << endl;
	}

	return n;
}

//void Serial::execute(void* arg){
//	int lenRead = 0;
//	while (!isStopped){
//			if ((lenRead = readMessage(&receiveBuffer)) < 0){
//				printf("receiving from %s failed\n,", SERIAL_INTERFACE_1);
//			}
//			printf("%c\n", receiveBuffer);
//			//receiveBuffer = 0;
//		}
//}
