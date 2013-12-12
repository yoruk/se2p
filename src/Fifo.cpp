#include "Fifo.h"
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

pthread_mutex_t m;
sem_t freefields;
sem_t busyfields;

//first free field
int head = 0;
//last busy field
int tail = 0;

Fifo::Fifo() {
	sem_init(&freefields, 1, BUFFER_LENGHT);
	sem_init(&busyfields, 1, 0);
	pthread_mutex_init(&m, NULL);
}

Fifo::~Fifo() {
	// TODO Auto-generated destructor stub
}

void Fifo::put(Puk element) {
	//warte auf freefield
	sem_wait(&freefields);
	//enter critical section
	pthread_mutex_lock(&m);

	buffer[head] = element;
	head++;
	if (head == BUFFER_LENGHT) {
		head = 0;
	}

	//leave critical section
	pthread_mutex_unlock(&m);
	//erhöhe anzahl elemente im puffer
	sem_post(&busyfields);
}

Puk Fifo::remove() {
	Puk ret_element;
	//warte auf busyfield
	sem_wait(&busyfields);
	//enter critical section
	pthread_mutex_lock(&m);
	ret_element = buffer[tail];
	tail++;
	if (tail == BUFFER_LENGHT) {
		tail = 0;
	}
	//leave critical section
	pthread_mutex_unlock(&m);
	//verringere anzahl elemenete im buffer
	sem_post(&freefields);
	return ret_element;
}


Puk* Fifo::get() {
	Puk* ret_element;
	//enter critical section
	pthread_mutex_lock(&m);
	ret_element = &buffer[tail];
	//leave critical section
	pthread_mutex_unlock(&m);
	//verringere anzahl elemenete im buffer
	return ret_element;
}
