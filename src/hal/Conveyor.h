#ifndef CONVEYOR_H_
#define CONVEYOR_H_

#include "../Mutex.h"

class Conveyor {
public:
	static Conveyor* getInstance();
	virtual ~Conveyor();

	int getDirection();
	int getSpeed();

	void moveRight();
	void moveLeft();
	void moveSlow();

	void conveyorStop();
	void conveyorContinue();

private:
	Conveyor();
};

#endif /* CONVEYOR_H_ */
