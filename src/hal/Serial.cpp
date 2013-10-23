#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <../Mutex.h>
#include <../Global.h>
#include "Serial.h"

static Mutex* mutex = new Mutex();
static Serial* serial;

Serial::Serial() {}

Serial::~Serial() {
	delete serial;
	serial = NULL;
}

Serial* Serial::getInstance() {
	mutex->lock();

	if (!serial) {
		serial = new Serial();
	}

	mutex->unlock();

	return serial;
}

int Serial::write_serial(char* buffer, int size, char interface[]) {
	int fd = 0;
	int n = 0;
	int bytesLeft = size;

	mutex->lock();

	fd = open(interface, O_RDWR | O_NOCTTY);
	if(fd == -1) {
		perror("write_serial: error, port cant be opened\n");fflush(stdout);
		return 0;
	}

//	else {
//		fcntl(fd, F_SETFL, 0);
//	}

	while(bytesLeft != 0) {
		n = write(fd, &buffer[size - bytesLeft], bytesLeft);
		if(n <= 0) {
			printf("write_serial: error during writing\n");fflush(stdout);
			close(fd);
			return n;
		}

		bytesLeft -= n;
	}

	close(fd);

	mutex->unlock();

	return n;
}

int Serial::read_serial(char* buffer, int size, char interface[]) {
	int fd = 0;
	int n = 0;
	int bytesLeft = size;

	mutex->lock();

	fd = open(interface, O_RDONLY | O_NOCTTY);
	if(fd == -1) {
		perror("read_serial: error, port cant be opened\n");fflush(stdout);
		return 0;
	}

	while(bytesLeft != 0) {
		n = read(fd, &buffer[size - bytesLeft], bytesLeft);
		if(n <= 0) {
			printf("read_serial: error during reading\n");fflush(stdout);
			close(fd);
			return n;
		}

		bytesLeft -= n;
	}

	close(fd);

	mutex->unlock();

	return n;
}

