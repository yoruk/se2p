#include <stdio.h>
#include <stdlib.h>
#include "Fifo.h"
#include "Puk.h"
#include "Global.h"

#define COUNT (BUFFER_LENGTH + 1)

void test_Fifo_start() {
	Fifo* fifo;
	Puk* puk;
	int i;

	printf("fifo test!\n\n");fflush(stdout);

	fifo = new Fifo();

	printf("trying to get from empty buffer = ");fflush(stdout);
	if(fifo->get() == NULL) {
		printf("failed\n");fflush(stdout);
	} else {
		printf("success\n");fflush(stdout);
	}

	printf("trying to remove from empty buffer = ");fflush(stdout);
	if(fifo->remove() == NULL) {
		printf("failed\n");fflush(stdout);
	} else {
		printf("success\n");fflush(stdout);
	}

	printf("\nbuffer length = %d\n", BUFFER_LENGTH);fflush(stdout);

	printf("putting %d puks into buffer\n", COUNT);fflush(stdout);
	for(i=0; i<COUNT; i++) {
		if(fifo->put(new Puk(i)) != 0) {
			printf("failed\n");fflush(stdout);
		} else {
			printf("puk id = %d\n", i);
		}
	}

	printf("removing %d puks from buffer\n", COUNT);fflush(stdout);
	for(i=0; i<COUNT; i++) {
		puk = fifo->remove();

		if(puk == NULL) {
			printf("failed\n");fflush(stdout);
		} else {
			printf("puk id = %d\n", puk->get_id());
		}
	}
}
