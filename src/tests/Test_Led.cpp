#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "../Mutex.h"
#include "../hal/Led.h"

#define RUNTIME 30
#define SLEEP 1

static Mutex* mutex;
static pthread_t t1, t2;
static unsigned char id1 = 0;
static unsigned char id2 = 1;
static bool run = true;

void* led_thread(void* arg) {
	Led* led = Led::getInstance();
	int id = (int)arg;

	while(run) {
			sleep(id+1);

			mutex->lock();

			if(id == 0){
				sleep(SLEEP);
				led->led_StartButton_On();
				sleep(SLEEP);
				led->led_ResetButton_On();
				sleep(SLEEP);
				led->led_Q1_On();
				sleep(SLEEP);
				led->led_Q2_On();
			} else {
				sleep(SLEEP);
				led->led_StartButton_Off();
				sleep(SLEEP);
				led->led_ResetButton_Off();
				sleep(SLEEP);
				led->led_Q1_Off();
				sleep(SLEEP);
				led->led_Q2_Off();
			}


			sleep(id+1);
			mutex->unlock();
	}

	pthread_exit(NULL);
}

void test_Led_start() {
	printf("Test_Util/test_Led_start()\n\n");fflush(stdout);

	mutex = new Mutex();

	pthread_create(&t1, NULL, &led_thread, (void*)id1);
	pthread_create(&t2, NULL, &led_thread, (void*)id2);

	sleep(RUNTIME);

	run = false;

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	delete mutex;
}
