#ifndef CONVEYOR_H_
#define CONVEYOR_H_

#include "../Mutex.h"
#include "hw.h"
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"

class Conveyor {
public:
	static Conveyor* getInstance();
	virtual ~Conveyor();

	int getDirection();
	int getSpeed();

	void moveRight();
	void moveLeft();
	void moveSlow();
	void moveFast();

	void conveyorStop();
	void conveyorContinue();

private:
	Conveyor();
};

#endif /* CONVEYOR_H_ */
