#include "Test_Led.h"

#define RUNTIME 30
#define SLEEP 1

static Led* led;
static Mutex* mutex;
static pthread_t t1, t2;
static unsigned int id0 = 0;
static unsigned int id1 = 1;
static bool run = true;

void* led_thread(void* arg) {
	int id = *((int*)arg);

	while(run) {
			sleep(id+1);

			mutex->lock();

			if(id == 0){
				printf("Light On!\n");fflush(stdout);

				sleep(SLEEP);
				led->led_StartButton_On();
				sleep(SLEEP);
				led->led_ResetButton_On();
				sleep(SLEEP);
				led->led_Q1_On();
				sleep(SLEEP);
				led->led_Q2_On();
			} else {
				printf("Light Off!\n");fflush(stdout);

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
	led = Led::getInstance();
	mutex = new Mutex();

	printf("Test_Util/test_Led_start()\n\n");fflush(stdout);

	pthread_create(&t1, NULL, &led_thread, (&id0));
	pthread_create(&t2, NULL, &led_thread, (&id1));

	sleep(RUNTIME);

	run = false;

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	delete mutex;
}
