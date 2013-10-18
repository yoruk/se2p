#ifndef TREADMILL_H_
#define TREADMILL_H_

#include "../Mutex.h"

class Treadmill {
public:
	static Treadmill* getInstance();
	virtual ~Treadmill();

	int status(unsigned int treadmill);

	void moveRight();

	void moveLeft();

	void moveSlow();

	void treadmilStop();

private:
	Treadmill();

};

#endif /* TREADMILL_H_ */
