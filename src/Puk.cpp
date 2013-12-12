#include "Puk.h"

int Puk::count = 0;

Puk::Puk() {
	count++;
	id = count;
	typ = 0;
	hoehenmessung = 0;
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
int Puk::get_hoehenmessung (){
	return hoehenmessung;
}
bool Puk::get_status_flach(){
	return status_flach;
}


void Puk::set_typ(char par){
	typ = par;
}
void Puk::set_hoehenmessung(int par){
	hoehenmessung = par;
}
void Puk::set_status_flach(bool par){
	status_flach = par;
}
