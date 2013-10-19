#include "Test_Mutex.h"
#include "Mutex.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define WAIT 1
#define RUNTIME 10

static pthread_t t1, t2;
static Mutex* mutex;
static int id1 = 1;
static int id2 = 2;
static bool run = true;

void* mutex_thread(void* arg) {
	 while(run){
		 mutex->lock();
		 printf("Thread %d enters critical section\n",arg); fflush(stdout);
		 printf("Thread %d left critical section \n",arg); fflush(stdout);
		 mutex->unlock();
		 sleep((int)arg);
	 }

	 pthread_exit(NULL);
}

int test_Mutex_start() {
	mutex = new Mutex();

	pthread_create(&t1,NULL,&mutex_thread,(void*) id1);
	pthread_create(&t2,NULL,&mutex_thread,(void*) id2);

	sleep(RUNTIME);

//	pthread_cancel(t1);
//	pthread_cancel(t2);
	run = false;

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	delete mutex;

	return EXIT_SUCCESS;
}

