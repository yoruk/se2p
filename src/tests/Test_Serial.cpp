#include "Test_Serial.h"

#define WAIT 2

static pthread_t t1, t2;
static Serial* serial;
static char send_buffer[] = "hello world!";
static char receive_buffer[32];

static void* sender(void* arg) {
	printf("sender: is running\n");fflush(stdout);

	serial->write_serial(send_buffer, sizeof(send_buffer), SERIAL_INTERFACE_1);

	printf("sender: message: %s\n", send_buffer);fflush(stdout);

	printf("sender: ended\n");fflush(stdout);
	pthread_exit(NULL);
}

static void* receiver(void* arg) {
	printf("receiver: is running\n");fflush(stdout);

	serial->read_serial(receive_buffer, sizeof(send_buffer), SERIAL_INTERFACE_2);

	printf("receiver: message: %s\n", receive_buffer);fflush(stdout);

	printf("receiver: ended\n");fflush(stdout);
	pthread_exit(NULL);
}

void test_Serial_start() {
	serial = Serial::getInstance();

	printf("serial test!\n");fflush(stdout);

	pthread_create(&t1, NULL, &receiver, NULL);
	sleep(WAIT);
	pthread_create(&t2, NULL, &sender, NULL);

//	sleep(10);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
