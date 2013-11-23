#include "Test_Gate.h"

#define RUNTIME 20
#define WAIT 1000000

static Gate* gate;
static Mutex* mutex;
static pthread_t t1, t2;
static unsigned int id0 = 0;
static unsigned int id1 = 1;
static bool run = true;

void* gate_thread(void* arg) {
	int id = *((int*)arg);
	int after;

	printf("Thread %d Memmory address of Gate: 0x%x\n", id, gate);fflush(stdout);

	while(run) {
			usleep(WAIT);

			mutex->lock();

			if(id == 0){
				printf("Thread %d is closing Gate\n", id);fflush(stdout);
				gate->close();
			} else {
				printf("Thread %d is opening Gate\n", id);fflush(stdout);
				gate->open();
			}

			after = gate->status();
			printf("Thread %d gate status after %d\n", id, after);fflush(stdout);

			usleep(WAIT);
			mutex->unlock();
	}

	printf("thread %d ended\n", id);fflush(stdout);
	pthread_exit(NULL);
}

void test_Gate_start() {
	gate = Gate::getInstance();
	mutex = new Mutex();

	pthread_create(&t1,NULL,&gate_thread,(void*) (&id0));
	pthread_create(&t2,NULL,&gate_thread,(void*) (&id1));

	sleep(RUNTIME);

//	pthread_cancel(t1);
//	pthread_cancel(t2);
	run = false;

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	delete mutex;
	delete gate;
}
