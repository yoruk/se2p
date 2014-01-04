#ifndef TRAFFICLIGHTBLINKEN_H_
#define TRAFFICLIGHTBLINKEN_H_

#include "HAWThread.h"

class TrafficLightBlinken: public thread::HAWThread {
public:
	TrafficLightBlinken();
	virtual ~TrafficLightBlinken();

protected:
	virtual void execute(void* arg);
	virtual void shutdown();
};

#endif /* TRAFFICLIGHTBLINKEN_H_ */
