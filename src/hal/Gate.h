#ifndef GATE_H_
#define GATE_H_

#include "../Mutex.h"

class Gate {
public:
	static Gate* getInstance();
    virtual ~Gate();
    int status();
    void close();
    void open();

protected:
    Gate();

private:
    static Gate* gate;
};

#endif /* GATE_H_ */
