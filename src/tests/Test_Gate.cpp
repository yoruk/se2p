#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../hal/Gate.h"

#define RUNTIME 10

static Mutex* mutex;
static pthread_t t1, t2;
static unsigned char id1 = 0;
static unsigned char id2 = 1;

Gate* gate = Gate::getInstance();

void* gate_thread(void* arg) {
	int id = ((int)arg)%2;
	unsigned char after;

	printf("Thread %d Memmory address of Gate: 0x%x\n",id, gate);fflush(stdout);

	while(true) {
			mutex->lock();

			if(id == 0){
				printf("Thread %d is closing Gate\n", id);fflush(stdout);
				gate->close();

			}else{
				printf("Thread %d is opening Gate\n", id);fflush(stdout);
				gate->open();
			}

			after = gate->status();
			printf("Thread %d gate status after %d\n", id, after);fflush(stdout);

			if(((after == 1) && (id == 0)) || ((after == 0) && (id == 1))) {
				printf("!!!\n");fflush(stdout);
			}

			mutex->unlock();

			sleep(id+1);
	}

	pthread_exit(NULL);
}

void test_Gate_start() {
	mutex = new Mutex();

	pthread_create(&t1,NULL,&gate_thread,(void*) id1);
	pthread_create(&t2,NULL,&gate_thread,(void*) id2);

	sleep(RUNTIME);

	pthread_cancel(t1);
	pthread_cancel(t2);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
}
