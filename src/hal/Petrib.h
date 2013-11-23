#ifndef PETRIB_H_
#define PETRIB_H_


#include <iostream>
#include "hw.h"
#include "Mutex.h"
#include "Conveyor.h"
#include "Gate.h"
#include "Led.h"
#include "TrafficLight.h"




class Petrib{
public:
	static Petrib* getInstance();
	void init_places();
	void process_transitions();
	void process_transitios();
	void calculate_outputs();
	void write_outputs();
	void NotifyReactor();
	void print_places();
	void read_inputs(int code, int value);
	virtual ~Petrib();

private:
	Petrib();
	Gate* gate;
	TrafficLight* trafficlight;
	Led* led;
	Conveyor* conv;

};

#endif /* PETRIB_H_ */
