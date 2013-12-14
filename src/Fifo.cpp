#include <stdlib.h>
#include <stdio.h>
#include "Fifo.h"

Fifo::Fifo() {
	Fifo::mutex = new Mutex();
	clear();
}

Fifo::~Fifo() {

}

void Fifo::clear() {
	int i;

	Fifo::read_idx = 0;
	Fifo::write_idx = 0;
	Fifo::count = 0;

	for(i=0; i<BUFFER_LENGTH; i++) {
		buffer[i] = NULL;
	}
}

int Fifo::put(Puk* p) {
	if(count < BUFFER_LENGTH) {

		// still space in buffer
		buffer[write_idx] = p;

		write_idx = (write_idx + 1) % BUFFER_LENGTH;
		count++;

		return 0;

	} else {

		// buffer is already full
		return 1;
	}
}

Puk* Fifo::remove() {
	Puk* tmp = new Puk(0);

	if(count == 0) {

		// buffer is empty
		return NULL;

	} else {

		// buffer has at least one puk
		*tmp = *buffer[read_idx];
		buffer[read_idx] = NULL;

		read_idx = (read_idx + 1) % BUFFER_LENGTH;
		count--;

		return tmp;
	}
}

Puk* Fifo::get() {
	if(count == 0) {

		// buffer is empty
		return NULL;

	} else {

		// buffer has at least one puk
		return buffer[read_idx];
	}
}
