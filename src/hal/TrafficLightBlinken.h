#ifndef TRAFFICLIGHTBLINKEN_H_
#define TRAFFICLIGHTBLINKEN_H_

#include "HAWThread.h"
#include "HWaccess.h"
#include "../Mutex.h"
#include "hw.h"
#include "Util.h"
#include <unistd.h>

class TrafficLightBlinken: public thread::HAWThread {
public:
	virtual ~TrafficLightBlinken();
	static TrafficLightBlinken* getInstance();

	void flashRedOn();

private:
	TrafficLightBlinken();
	static TrafficLightBlinken* instance;
protected:
	virtual void execute(void* arg);
	virtual void shutdown();
};

#endif /* TRAFFICLIGHTBLINKEN_H_ */
