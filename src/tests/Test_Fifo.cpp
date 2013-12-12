#include "Test_Fifo.h"

void test_Fifo_start() {

	Fifo* fifo = new Fifo();

	Puk* puk1 = new Puk();
	puk1->set_hoehenmessung1(8);
	puk1->set_typ(1);

	Puk* puk2 = new Puk();
	puk2->set_hoehenmessung1(4);
	puk2->set_typ(3);

	Puk* puk3 = new Puk();
	puk3->set_hoehenmessung1(12);
	puk3->set_typ(2);

	Puk* puk4 = new Puk();
	puk4->set_hoehenmessung1(16);
	puk4->set_typ(3);

	fifo->put(*puk1);
	fifo->put(*puk2);
	fifo->put(*puk3);
	fifo->put(*puk4);

	//printf("Pukid %d \n",fifo->remove().get_id());fflush(stdout);

	printf("Puk1 Hoehe BEFORE %d \n",puk1->get_hoehenmessung1());fflush(stdout);
	printf("Puk2 Hoehe BEFORE %d \n",puk2->get_hoehenmessung1());fflush(stdout);
	printf("Puk1 Hoehe BEFORE %d \n",puk3->get_hoehenmessung1());fflush(stdout);
	printf("Puk2 Hoehe BEFORE %d \n",puk4->get_hoehenmessung1());fflush(stdout);

	//fifo->remove();

	printf("Puk2 Hoehe BEFORE %d \n",fifo->get()->get_hoehenmessung1());fflush(stdout);

	Puk* tmp2 = fifo->get();
	tmp2->set_hoehenmessung1(10);

	printf("Puk2 Hoehe BEFORE %d \n",fifo->get()->get_hoehenmessung1());fflush(stdout);
}
