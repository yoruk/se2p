#ifndef GATE_H_
#define GATE_H_

#include "../Mutex.h"
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "hw.h"



class Gate {
public:
	static Gate* getInstance();
    virtual ~Gate();
    int status();
    void close();
    void open();

private:
    Gate();
};

#endif /* GATE_H_ */
