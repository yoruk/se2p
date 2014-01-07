#ifndef CONVEYOR_H_
#define CONVEYOR_H_


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

	void resetConveyorBits();

private:
	Conveyor();
};

#endif /* CONVEYOR_H_ */
