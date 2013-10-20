#include "Mutex.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/// mutex-constructor
Mutex::Mutex() {
	if(pthread_mutex_init(&mutex, NULL) != 0) {
		perror("mutex failed to init\n");
		exit(EXIT_FAILURE);
	}
}

/// mutex-deconstructor
Mutex::~Mutex() {
	if(pthread_mutex_destroy(&mutex) != 0) {
		perror("mutex failed to destroy\n");
		exit(EXIT_FAILURE);
	}
}

/// locks the mutex
void Mutex::lock() {
    if(pthread_mutex_lock(&mutex) != 0) {
		perror("mutex failed to lock\n");
		exit(EXIT_FAILURE);
    }
}

/// unlocks the mutex
void Mutex::unlock() {
	if(pthread_mutex_unlock(&mutex) != 0) {
		perror("mutex failed to unlock\n");
		exit(EXIT_FAILURE);
	}
}
