#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "../hal/Gate.h"

#define RUNTIME 200
#define WAIT 1000000

static Gate* gate;
static Mutex* mutex;
static pthread_t t1, t2;
static int id0 = 0;
static int id1 = 1;
static bool run = true;

//static void* gate_simple_thread_1(void* arg) {
//	while(run) {
//		usleep(WAIT);
//		mutex->lock();
//
//		printf("thread 1\n");fflush(stdout);
//		gate->open();
//
//		usleep(WAIT);
//		mutex->unlock();
//	}
//
//	pthread_exit(NULL);
//}

//static void* gate_simple_thread_2(void* arg) {
//	while(run) {
//		usleep(WAIT);
//		mutex->lock();
//
//		printf("thread 2\n");fflush(stdout);
//		gate->close();
//
//		usleep(WAIT);
//		mutex->unlock();
//	}
//
//	pthread_exit(NULL);
//}


static void* gate_simple_single_thread(void* arg) {
	int id = *((int*)arg);

	while(run) {
		usleep(WAIT);
		mutex->lock();


		printf("thread %d\n", id);fflush(stdout);
		if(id == 0) {
			gate->open();
		} else {
			gate->close();
		}

		usleep(WAIT);
		mutex->unlock();
	}

	pthread_exit(NULL);
}

void test_Simple_Gate_start() {
	gate = Gate::getInstance();
	mutex = new Mutex();

	//pthread_create(&t1, NULL, &gate_simple_thread_1, NULL);
	//pthread_create(&t2, NULL, &gate_simple_thread_2, NULL);

	pthread_create(&t1, NULL , &gate_simple_single_thread, (void*)(&id0));
	pthread_create(&t1, NULL , &gate_simple_single_thread, (void*)(&id1));

	sleep(RUNTIME);

	run = false;

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	gate->close();
	delete mutex;
	delete gate;
}
