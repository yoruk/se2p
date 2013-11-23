#ifndef TRAFFICLIGHT_H_
#define TRAFFICLIGHT_H_

#include "../Mutex.h"
#include "hw.h"
#include "Util.h"

class TrafficLight {
public:
	static TrafficLight* getInstance();
	virtual ~TrafficLight();

	int statusRed();
	int statusYellow();
	int statusGreen();

	void redOn();
	void redOff();

	void yellowOn();
	void yellowOff();

	void greenOn();
	void greenOff();

private:
	TrafficLight();
};
#endif /* TRAFFICLIGHT_H_ */
