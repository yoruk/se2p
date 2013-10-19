#ifndef SERIAL_H_
#define SERIAL_H_

#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream.h>

class Serial {
public:
	Serial();
	virtual ~Serial();

	int sendMessage(char message);
	int readMessage(char* buffer);

private:
	int sd;
};

#endif /* SERIAL_H_ */
