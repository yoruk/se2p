#ifndef SERIELL_H_
#define SERIELL_H_

#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream.h>

#define SCHNITTSTELLE1 "/dev/ser1"
#define SCHNITTSTELLE2 "/dev/ser2"

class Seriell {
public:
	Seriell();
	virtual ~Seriell();

	int sendMessage(char message);
	int readMessage(char* buffer);

private:
	int sd;
};

#endif /* SERIELL_H_ */
