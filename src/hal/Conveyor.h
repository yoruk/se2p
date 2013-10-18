#ifndef CONVEYOR_H_
#define CONVEYOR_H_

#include "../Mutex.h"

class Conveyor {
public:
	static Conveyor* getInstance();
	virtual ~Conveyor();

	int statusLeft();
	int statusRight();
	int statusSlow();
	int statusStop();

	void moveRight();

	void moveLeft();

	void moveSlow();

	void conveyorStop();

private:
	Conveyor();

};

#endif /* CONVEYOR_H_ */
