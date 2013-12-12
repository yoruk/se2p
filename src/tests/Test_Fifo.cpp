#include "Test_Fifo.h"

void test_Fifo_start() {

	Fifo* fifo = new Fifo();

	Puk* puk1 = new Puk();
	puk1->set_hoehenmessung(8);
	puk1->set_status_flach(false);
	puk1->set_typ(1);

//	Puk* puk2 = new Puk();
//	puk1->set_hoehenmessung(4);
//	puk1->set_status_flach(true);
//	puk1->set_typ(3);
//
//	Puk* puk3 = new Puk();
//	puk1->set_hoehenmessung(8);
//	puk1->set_status_flach(false);
//	puk1->set_typ(2);
//
//	Puk* puk4 = new Puk();
//	puk1->set_hoehenmessung(4);
//	puk1->set_status_flach(true);
//	puk1->set_typ(3);

	fifo->put(*puk1);
//	fifo->put(*puk2);
//	fifo->put(*puk3);
//	fifo->put(*puk4);

	printf("Pukid %d",fifo->remove().get_id());fflush(stdout);




}
