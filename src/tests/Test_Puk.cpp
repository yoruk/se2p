#include <stdio.h>
#include <stdlib.h>
#include "../Puk.h"

#define ID 16383
#define TYPE 0
#define HEIGHT 65535

void test_Puk_start() {
	Puk* p1;
	Puk* p2;
	int val = 0;

	printf("puk test!\n\n");fflush(stdout);

	printf("Creating puk objectb\n");fflush(stdout);
	printf("id = %d, 0x%x\n", ID, ID);fflush(stdout);
	printf("type = %d, 0x%x\n", TYPE, TYPE);fflush(stdout);
	printf("height = %d, 0x%x\n", HEIGHT, HEIGHT);fflush(stdout);
	p1 = new Puk(ID);
	p1->set_typ(TYPE);
	p1->set_hoehenmessung1(HEIGHT);

	val = Puk::pukToInt(p1);
	printf("\ntesting: int pukToInt(Puk* p) = 0x%x\n", val);fflush(stdout);

	printf("\ntesting: int intToPuk(int i)\n");fflush(stdout);
	printf("puk data:\n");fflush(stdout);
	p2 = Puk::intToPuk(val);
	printf("id = %d, 0x%x\n", p2->get_id(), p2->get_id());fflush(stdout);
	printf("type = %d, 0x%x\n", p2->get_typ(), p2->get_typ());fflush(stdout);
	printf("height = %d, 0x%x\n", p2->get_hoehenmessung1(), p2->get_hoehenmessung1());fflush(stdout);
}
