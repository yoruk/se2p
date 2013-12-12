#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../Global.h"
#include "../hal/Serial.h"

#define WAIT 2
#define RUNTIME 30

static pthread_t t1, t2;
static int run = TRUE;
static Serial* serial;
static char send_buffer[] = "hello world";
static char receive_buffer[32];

static void* sender(void* arg) {
	printf("DEBUG:Test_Serial: sender: is running\n");fflush(stdout);

	while(run) {
		serial->write_serial((unsigned char*)send_buffer, sizeof(send_buffer));

		printf("DEBUG:Test_Serial: sender: message = %s\n", send_buffer);fflush(stdout);

		sleep(WAIT);
	}

	printf("DEBUG:Test_Serial: sender: ended\n");fflush(stdout);

	pthread_exit(NULL);
}

static void* receiver(void* arg) {
	printf("DEBUG:Test_Serial: receiver: is running\n");fflush(stdout);

	while(run) {
		serial->read_serial((unsigned char*)receive_buffer, sizeof(send_buffer));

		printf("DEBUG:Test_Serial: receiver: message: %s\n", receive_buffer);fflush(stdout);
	}

	printf("DEBUG:Test_Serial: receiver: ended\n");fflush(stdout);

	pthread_exit(NULL);
}

void test_Serial_start() {
	serial = Serial::getInstance();

	printf("serial test!\n");fflush(stdout);

	if(serial->open_serial() != 0) {
		perror("Test_Serial: ERROR, couldn't open port");fflush(stdout);
	}

	sleep(10);

	pthread_create(&t1, NULL, &receiver, NULL);
	sleep(WAIT);
	pthread_create(&t2, NULL, &sender, NULL);

	sleep(RUNTIME);
	run = FALSE;
	pthread_cancel(t1);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	serial->close_serial();
}
