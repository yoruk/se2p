#ifndef TRAFFICLIGHTGELBBLINKEN_H_
#define TRAFFICLIGHTGELBBLINKEN_H_

#include "HAWThread.h"
#include "HWaccess.h"
#include "../Mutex.h"
#include "hw.h"
#include "Util.h"
#include <unistd.h>

class TrafficLightGelbBlinken: public thread::HAWThread  {
public:
	TrafficLightGelbBlinken();
	virtual ~TrafficLightGelbBlinken();
	void flashRedOn();

protected:
	virtual void execute(void* arg);
	virtual void shutdown();
};

#endif /* TRAFFICLIGHTGELBBLINKEN_H_ */
