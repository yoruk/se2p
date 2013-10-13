#include "Mutex.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

Mutex::Mutex() {
	if(pthread_mutex_init(&mutex, NULL) != 0) {
		perror("mutex failed to init\n");
		exit(EXIT_FAILURE);
	}
}

Mutex::~Mutex() {
	if(pthread_mutex_destroy(&mutex) != 0) {
		perror("mutex failed to destroy\n");
		exit(EXIT_FAILURE);
	}
}

void Mutex::lock() {
    if(pthread_mutex_lock(&mutex) != 0) {
		perror("mutex failed to lock\n");
		exit(EXIT_FAILURE);
    }
}

void Mutex::unlock() {
	if(pthread_mutex_unlock(&mutex) != 0) {
		perror("mutex failed to unlock\n");
		exit(EXIT_FAILURE);
	}
}
