#ifndef SENSORIK_H_
#define SENSORIK_H_

#include "HAWThread.h"
#include "../Mutex.h"
#include "hw.h"
#include <stdint.h>

const struct sigevent * ISR(void *arg, int id);

class Sensorik: public thread::HAWThread {
private:
	static Sensorik* instance;

	int isr_Chid;
	int interruptId;
	struct sigevent event;

	int sensorik_Chid;
	int sensorik_Coid;

	uint8_t portBstatus;
	uint8_t portCstatus;

	Sensorik();

public:
	~Sensorik();
	static Sensorik* getInstance();

	void stop();
	int getSignalChid();
	int getHeight();
	int getHeightPuckType();
protected:
	virtual void execute(void* arg);
	virtual void shutdown();

private:
	void initInterrupts();
};

#endif /* SENSORIK_H_ */
