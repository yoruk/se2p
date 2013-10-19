#include "Serial.h"
#include "../Global.h"

Serial::Serial() {
	//O_RDWR: open for reading and writing.
	sd = (open(SERIAL_INTERFACE_1, O_RDWR));
}

Serial::~Serial() {
	close(sd);
}

int Serial::sendMessage(char message) {
	return write(sd, &message, sizeof(message));
}

int Serial::readMessage(char* buffer) {
	return readcond(sd, buffer, sizeof(buffer), 1, 0, 15);
}
