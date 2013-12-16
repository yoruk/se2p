#ifndef PETRI_CONTROLLER_2_H_
#define PETRI_CONTROLLER_2_H_

#include "HAWThread.h"
#include "HWaccess.h"
#include <iostream>
#include "hw.h"
#include "Mutex.h"
#include "Gate.h"
#include "Led.h"
#include "Dispatcher.h"
#include "Timer.h"
#include "TimerHandler.h"
#include "Puk.h"

#define N_PLACE_2 19

class Petri_Controller_2: public thread::HAWThread {
public:
	static Petri_Controller_2* getInstance();

	virtual ~Petri_Controller_2();

private:

	Sensorik* petri_controller_2_sen;
	int petri_controller_2_dispatcher_Chid;
	int petri_controller_2_dispatcher_Coid;
	int petri_controller_2_sensorik_Chid;
	Puk puk_c2;
	int puk_tmp_type;

	Petri_Controller_2();

	Gate* gate;
	Led* led;
	Dispatcher* disp_petri_controller_2;
	TimerHandler* timer;
	Timer* timer_C2_GateClose;
	Timer* timer_C2_SlideFull;
	Timer* timer3_C2;

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

#endif /* PETRI_CONTROLLER_2_H_ */
