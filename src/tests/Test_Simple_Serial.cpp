#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include "../Global.h"

#define SERIAL_INTERFACE_SENDER "/dev/ser1"
#define SERIAL_INTERFACE_RECEIVER "/dev/ser2"

#define WAIT 2
#define RUNTIME 60

static pthread_t snd;
static pthread_t rcv;
static int fd_sender = 0;
static int fd_receiver = 0;
static char send_buffer[] = "hello world";
static char receive_buffer[32];

int openSerial(int* fd, char* interface) {
	struct termios new_port_settings;
	int res = 0;

	*fd = open(interface, O_RDWR | O_NOCTTY);
	if(*fd == -1) {
		perror("openSerial: ERROR, port can't be opened");fflush(stdout);
		return 1;
	}

	// flush i/o-buffers
	//tcflush(fd, TCIOFLUSH);

	// load current settings
	res = tcgetattr(*fd, &new_port_settings);
	if(res == -1) {
			perror("openSerial: ERROR, couldn't load current port settings");fflush(stdout);
			close(*fd);
			return(1);
	}

//	// set new port settings
//	memset(&new_port_settings, 0, sizeof(new_port_settings));
//
//	new_port_settings.c_cflag = SERIAL_SPEED | CS8 | CLOCAL | CREAD;
//	new_port_settings.c_iflag = IGNPAR;
//	new_port_settings.c_oflag = 0;
//	new_port_settings.c_lflag = 0;
//	new_port_settings.c_cc[VMIN] = 1;	/* block until n bytes are received */

	// set baudrate
	cfsetispeed(&new_port_settings, B9600);
	cfsetospeed(&new_port_settings, B9600);

	// 8 data, 1 stop, no parity
	new_port_settings.c_cflag &= ~CSIZE;
	new_port_settings.c_cflag &= ~CSTOPB;
	new_port_settings.c_cflag &= ~PARENB;
	new_port_settings.c_cflag |= CS8;

	// raw input
	new_port_settings.c_lflag &= ~(ICANON | ECHO | ISIG | ECHOE | ECHOK | ECHONL);

	// raw output
	new_port_settings.c_oflag &= ~(OPOST);

	// enable receiver and set local mode
	new_port_settings.c_cflag |= (CLOCAL | CREAD);

	// write new port settings
	res = tcsetattr(*fd, TCSANOW, &new_port_settings);
	if(res == -1) {
		perror("Serial: ERROR, couldn't write new port settings");fflush(stdout);
		close(*fd);
		return(1);
	}

	return 0;
}

int closeSerial(int* fd, char* interface) {
	if(close(*fd) != 0) {
		perror("closeSerial: ERROR, port can't be closed");fflush(stdout);
		return 1;
	}

	return 0;
}

static void* sender(void* fd) {
	int n = 0;
	int bytesLeft = sizeof(send_buffer);

	printf("sender: is running\n");fflush(stdout);

	while(TRUE) {

		while(bytesLeft != 0) {
			n = write(fd_sender, &send_buffer[sizeof(send_buffer) - bytesLeft], bytesLeft);
			if(n <= 0) {
				perror("sender: ERROR, couldn't write");fflush(stdout);

				exit(EXIT_FAILURE);
			}

			bytesLeft -= n;
		}

		printf("sender: message: %s\n", send_buffer);fflush(stdout);
		bytesLeft = sizeof(send_buffer);

		sleep(WAIT);
	}

	printf("sender: ended\n");fflush(stdout);
	pthread_exit(NULL);
}

static void* receiver(void* arg) {
	int n = 0;
	int bytesLeft = sizeof(send_buffer);

	printf("receiver: is running\n");fflush(stdout);

	while(TRUE) {

		while(bytesLeft != 0) {
			n = read(fd_receiver, &receive_buffer[sizeof(send_buffer) - bytesLeft], bytesLeft);
			if(n <= 0) {
				perror("sender: ERROR, couldn't read");fflush(stdout);

				exit(EXIT_FAILURE);
			}

			bytesLeft -= n;
		}

		printf("receiver: message: %s\n", receive_buffer);fflush(stdout);
		bytesLeft = sizeof(send_buffer);
	}

	printf("receiver: ended\n");fflush(stdout);
	pthread_exit(NULL);
}

void test_Simple_Serial_start() {
	printf("serial test!\n");fflush(stdout);

	if(openSerial(&fd_sender, (char*)SERIAL_INTERFACE_SENDER) != 0) {
		perror("test_Simple_Serial_start: ERROR, sender port can't be opened");fflush(stdout);
	}

//	if(openSerial(&fd_receiver, (char*)SERIAL_INTERFACE_RECEIVER) != 0) {
//		perror("test_Simple_Serial_start: ERROR, receiver port can't be opened");fflush(stdout);
//	}

//	pthread_create(&rcv, NULL, &receiver, NULL);
//	sleep(WAIT);
	pthread_create(&snd, NULL, &sender, NULL);

	sleep(RUNTIME);

//	pthread_cancel(rcv);
//	pthread_join(rcv, NULL);
	pthread_cancel(snd);
	pthread_join(snd, NULL);

	if(closeSerial(&fd_sender, (char*)SERIAL_INTERFACE_SENDER) != 0) {
		perror("test_Simple_Serial_start: ERROR, sender port can't be closed");fflush(stdout);
	}

//	if(closeSerial(&fd_receiver, (char*)SERIAL_INTERFACE_RECEIVER) != 0) {
//		perror("test_Simple_Serial_start: ERROR, receiver port can't be closed");fflush(stdout);
//	}
}
