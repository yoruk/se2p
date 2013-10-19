#include "Seriell.h"

Seriell::Seriell() {
	//O_RDWR: open for reading and writing.
	sd = (open(SCHNITTSTELLE1, O_RDWR));
}

Seriell::~Seriell() {
	close(sd);
}

int Seriell::sendMessage(char message) {
	return write(sd, &message, sizeof(message));
}

int Seriell::readMessage(char* buffer) {
	return readcond(sd, buffer, sizeof(buffer), 1, 0, 15);
}
