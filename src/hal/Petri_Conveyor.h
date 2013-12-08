#ifndef PETRI_CONVEYOR_H_
#define PETRI_CONVEYOR_H_

#include "HAWThread.h"
#include "HWaccess.h"
#include <iostream>
#include "hw.h"
#include "Mutex.h"
#include "Conveyor.h"
#include "Dispatcher.h"

#define CONVEYOR_N_PLACE 5

class Petri_Conveyor: public thread::HAWThread {
public:

	static Petri_Conveyor* getInstance();

	virtual ~Petri_Conveyor();

private:

	int conveyor_dispatcher_Chid;
	Petri_Conveyor();
	Conveyor* conveyor;
	Dispatcher* conveyor_dispatcher;


	void init_places();
	void process_transitions();
	void calculate_outputs();
	void write_outputs();
	void NotifyReactor();
	void print_places();

	void set_conveyor_inputs();
	void set_conveyor_outputs();

	void reset_conveyor_inputs();
	void reset_conveyor_outputs();
protected:
    virtual void execute(void* arg);
    virtual void shutdown();
};

#endif /* PETRI_CONVEYOR_H_ */
