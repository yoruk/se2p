#include "Puk.h"

int Puk::count = -BUFFER_LENGHT;

Puk::Puk() {
	count++;
	id = count;
	typ = 0;
	hoehenmessung1 = 0;
	hoehenmessung2 = 0;
	status_flach = false;
}

Puk::~Puk() {
	// TODO Auto-generated destructor stub
}

int Puk::get_id(){
	return id;
}
char Puk::get_typ(){
	return typ;
}
int Puk::get_hoehenmessung1 (){
	return hoehenmessung1;
}

int Puk::get_hoehenmessung2 (){
	return hoehenmessung2;
}

bool Puk::get_status_flach(){
	return status_flach;
}


void Puk::set_typ(char par){
	typ = par;
}
void Puk::set_hoehenmessung1(int par){
	hoehenmessung1 = par;
}

void Puk::set_hoehenmessung2(int par){
	hoehenmessung2 = par;
}

void Puk::set_status_flach(bool par){
	status_flach = par;
}
