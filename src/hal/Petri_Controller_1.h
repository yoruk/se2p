#ifndef PETRI_CONTROLLER_1_H_
#define PETRI_CONTROLLER_1_H_

#include "HAWThread.h"
#include "HWaccess.h"
#include <iostream>
#include "hw.h"
#include "Mutex.h"
#include "Gate.h"
#include "Led.h"
#include "Dispatcher.h"

#define N_PLACE 26

class Petri_Controller_1: public thread::HAWThread {
public:
	static Petri_Controller_1* getInstance();

	virtual ~Petri_Controller_1();

private:

	int petri_controller_1_dispatcher_Chid;
	int petri_controller_1_dispatcher_Coid;
	int petri_controller_1_sensorik_Chid;

	Petri_Controller_1();

	Gate* gate1;
	Led* led;
	Dispatcher* disp_petri_controller_1;


	void init_places();
	void process_transitions();
	void calculate_outputs();
	void write_outputs();
	void NotifyReactor();
	void print_places();


	void setInputs();
	void setOutputs();


protected:
    virtual void execute(void* arg);
    virtual void shutdown();



};

#endif /* PETRI_CONTROLLER_1_H_ */