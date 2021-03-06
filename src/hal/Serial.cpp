#include "Serial.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
#include <fcntl.h>
#include <../Global.h>
#include <../Mutex.h>


/// This class's methods can transfer byte(s)
/// over the serial interface

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Serial* serial; /// the Serial object itself

/// Serial-constructor
Serial::Serial() {}

/// Serial-deconstructor
Serial::~Serial() {
	close_serial();

	delete serial;
	serial = NULL;
}

/// returns the only running instance of Serial
Serial* Serial::getInstance() {
	mutex->lock();

	if (!serial) {
		serial = new Serial();
	}

	mutex->unlock();

	return serial;
}

/// opens and sets up the serial port
///
/// \return	0=successful, 1=failed
int Serial::open_serial() {
	int res = 0;

	//printf("DEBUG:Serial: opening port\n");fflush(stdout);

/***** start critical section *****/
	mutex->lock();

	// only open if not opened before
	if(!init_done) {

		// open port
		fd = open(SERIAL_INTERFACE, O_RDWR | O_NOCTTY);
		if(fd == -1) {
			perror("Serial: ERROR, port couldn't be opened");fflush(stdout);
			mutex->unlock();
			return(1);
		}

		// flush i/o-buffers
		tcflush(fd, TCIOFLUSH);

		// load current port settings
		res = tcgetattr(fd, &port_settings);
		if(res == -1) {
				perror("Serial: ERROR, couldn't load current port settings");fflush(stdout);
				close(fd);
				mutex->unlock();
				return(1);
		}

//		// set new port settings
//		memset(&new_port_settings, 0, sizeof(new_port_settings));
//
//		new_port_settings.c_cflag = SERIAL_SPEED | CS8 | CLOCAL | CREAD;
//		new_port_settings.c_iflag = IGNPAR;
//		new_port_settings.c_oflag = 0;
//		new_port_settings.c_lflag = 0;
//		new_port_settings.c_cc[VMIN] = 1;	/* block until n bytes are received */

		// set baudrate
		cfsetispeed(&port_settings, SERIAL_SPEED);
		cfsetospeed(&port_settings, SERIAL_SPEED);

		// 8 data, 1 stop, no parity
		port_settings.c_cflag &= ~CSIZE;
		port_settings.c_cflag &= ~CSTOPB;
		port_settings.c_cflag &= ~PARENB;
		port_settings.c_cflag |= CS8;

		// raw input
		port_settings.c_lflag &= ~(ICANON | ECHO | ISIG | ECHOE | ECHOK | ECHONL);

		// raw output
		port_settings.c_oflag &= ~(OPOST);

		// enable receiver and set local mode
		port_settings.c_cflag |= (CLOCAL | CREAD);

		// write new port settings
		res = tcsetattr(fd, TCSANOW, &port_settings);
		if(res == -1) {
			perror("Serial: ERROR, couldn't write new port settings");fflush(stdout);
			close(fd);
			mutex->unlock();
			return(1);
		}

		init_done = TRUE;
	}

/***** end critical section *****/
	mutex->unlock();

	return 0;
}

/// closes the serial port
///
/// \return	0=successful, 1=failed
int Serial::close_serial() {
	//printf("DEBUG:Serial: closing port\n");fflush(stdout);

/***** start critical section *****/
	mutex->lock();

	// close serial port
	if(close(fd) != 0) {
		perror("Serial: ERROR, couldn't close port");fflush(stdout);
		return 1;
	}

	init_done = FALSE;

/***** end critical section *****/
	mutex->unlock();

	return 0;
}

/// writes a single or multiples bytes
/// to the serial port
///
/// \param buffer pointer to the data which should be transfered
/// \param size length of the data which should be transfered
///
/// \return	bytes being written
int Serial::write_serial(unsigned char* buffer, int size) {
	int n = 0;
	int bytesLeft = size;

//	if(!init_done) {
//		open_serial();
//	}

	//printf("DEBUG:Serial: writing to port\n");fflush(stdout);

	while(bytesLeft != 0) {
		n = write(fd, &buffer[size - bytesLeft], bytesLeft);
		if(n <= 0) {
			perror("Serial: ERROR, couldn't write properly");fflush(stdout);
			return n;
		}

		bytesLeft -= n;
	}

	return size - bytesLeft;
}

/// read a single or multiples bytes
/// from the serial port
///
/// \param buffer pointer to the localtion where data should be written
/// \param size length of the data which should be written
///
/// \return	bytes being written
int Serial::read_serial(unsigned char* buffer, int size) {
	int n = 0;
	int bytesLeft = size;

//	if(!init_done) {
//		open_serial();
//	}

	//printf("DEBUG:Serial: reading from port\n");fflush(stdout);

	while(bytesLeft != 0) {
		n = read(fd, &buffer[size - bytesLeft], bytesLeft);
		if(n <= 0) {
			perror("Serial: ERROR, couldn't read properly");fflush(stdout);
			return n;
		}

		bytesLeft -= n;
	}

	return size - bytesLeft;
}

