#ifndef PETRI_CONTROLLER_1_H_
#define PETRI_CONTROLLER_1_H_

#include "HAWThread.h"
#include "Sensorik.h"
#include "Gate.h"
#include "Led.h"
#include "Timer.h"
#include "TimerHandler.h"
#include "Dispatcher.h"

#define N_PLACE 38

class Petri_Controller_1: public thread::HAWThread {
public:
	static Petri_Controller_1* getInstance();

	virtual ~Petri_Controller_1();
	void timer_PauseAll();
	void timer_ContinueAll();

private:

	int petri_controller_1_dispatcher_Chid;
	int petri_controller_1_dispatcher_Coid;
	int petri_controller_1_sensorik_Chid;
	Sensorik* petri_controller_1_sen;

	Petri_Controller_1();

	Gate* gate1;
	Led* led;
	Dispatcher* disp_petri_controller_1;

	TimerHandler* timer_c1;
	Timer* timer_Gate;
	Timer* timer_move;
	Timer* timer_C1_SlideFull;
	Timer* timer_C1_removed_LSH;
	Timer* timer_C1_removed_LSW;
	Timer* timer_C1_removed_LSEND;

	void init_places();
	void process_transitions();
	void calculate_outputs();
	void write_outputs();
	void NotifyReactor();
	void print_places();

	void setInputs();


protected:
    virtual void execute(void* arg);
    virtual void shutdown();



};

#endif /* PETRI_CONTROLLER_1_H_ */
