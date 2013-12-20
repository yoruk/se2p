#include "Puk.h"
#include <stdio.h>
int Puk::count = 0;

Puk::Puk() {
	count++;
	id = count;
	typ = 0;
	hoehenmessung1 = 0;
	hoehenmessung2 = 0;
}

Puk::Puk(int i) {
	id = i;
	typ = 0;
	hoehenmessung1 = 0;
	hoehenmessung2 = 0;
}

Puk::~Puk() {
	// TODO Auto-generated destructor stub
}

int Puk::get_id() {
	return id;
}
char Puk::get_typ() {
	return typ;
}
int Puk::get_hoehenmessung1() {
	return hoehenmessung1;
}

int Puk::get_hoehenmessung2() {
	return hoehenmessung2;
}

void Puk::set_typ(char par) {
	typ = par;
}
void Puk::set_hoehenmessung1(int par) {
	hoehenmessung1 = par;
}

void Puk::set_hoehenmessung2(int par) {
	hoehenmessung2 = par;
}

// puk_id = 14bit
// puk_type = 2bit
// puk_height = 16bit

/// stuffs all the values of a Puk object from first conveyor
/// into a single int.
///
/// \param p a pointer to the input puk
///
/// \return the in that has the Puk values
unsigned int Puk::pukToInt(Puk* p) {
	unsigned int val = 0;

	val |= p->get_id();
	val <<= PUK_TYPE_LENGTH;

	val |= p->get_typ();
	val <<= PUK_HEIGHT_LENGTH;

	val |= p->get_hoehenmessung1();

	return val;
}

/// reads all the values of a Puk from an int
/// and generates a new Puk.
///
/// \param i the int that holds the Puk values
///
/// \return a new Puk
Puk* Puk::intToPuk(unsigned int i) {
	Puk* p = new Puk(i >> (PUK_TYPE_LENGTH + PUK_HEIGHT_LENGTH));

	p->set_typ((i >> PUK_HEIGHT_LENGTH) & PUK_TYPE_MASK);
	p->set_hoehenmessung1(i & PUK_HEIGHT_MASK);

	return p;
}

void Puk::print_puk(){
	printf("Puk(%d) Hoehe:%d  Typ:%d   ", id,hoehenmessung1,typ);fflush(stdout);
}
