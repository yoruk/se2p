#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "HAWThread.h"
#include "HWaccess.h"
#include "../Mutex.h"
#include "hw.h"
#include "Global.h"
#include "Sensorik.h"

namespace hal {
class Dispatcher: public thread::HAWThread {

public:
	virtual ~Dispatcher();
	static Dispatcher* getInstance();

private:
	int signalChid;
	static Dispatcher* instance;
	Dispatcher();
	void setSensor(int code, int value);

protected:
    virtual void execute(void* arg);
    virtual void shutdown();

	};
}
#endif /* DISPATCHER_H_ */
