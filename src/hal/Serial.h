#ifndef SERIAL_H_
#define SERIAL_H_

#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream.h>
#include <cstdlib>
#include <stdbool.h>
#include "Global.h"
#include "hw.h"

class Serial {
public:
	static Serial* getInstance();
	virtual ~Serial();

	int sendMessage(unsigned char message[]);
	int readMessage(char* buf);

private:
	Serial();
	int sd;
	int fileDescriptor;
	bool isStopped;
};

#endif /* SERIAL_H_ */
