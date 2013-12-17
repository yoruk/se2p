#ifndef PETRI_TRAFFICLIGHT_H_
#define PETRI_TRAFFICLIGHT_H_

#include "HAWThread.h"
#include "HWaccess.h"
#include <iostream>
#include "hw.h"
#include "Mutex.h"
#include "TrafficLight.h"
#include "Dispatcher.h"
#include "TrafficLightBlinken.h"
#include "TrafficLightGelbBlinken.h"

#define TRAFFICLIGHT_N_PLACE 5
class Petri_TrafficLight: public thread::HAWThread {

public:

	static Petri_TrafficLight* getInstance();

	virtual ~Petri_TrafficLight();

private:

	int trafficlight_dispatcher_Chid;
	Petri_TrafficLight();
	TrafficLight* trafficlight;
	TrafficLightBlinken* trafficlightblinken;
	TrafficLightGelbBlinken* trafficlightgelb;
	Dispatcher* trafficlight_dispatcher;


	void init_places();
	void process_transitions();
	void calculate_outputs();
	void write_outputs();
	void NotifyReactor();
	void print_places();

	void set_trafficlight_inputs();
	void set_trafficlight_outputs();
	void reset_trafficlight_outputs();
	void reset_trafficlight_inputs();

protected:
    virtual void execute(void* arg);
    virtual void shutdown();
};

#endif /* PETRI_TRAFFICLIGHT_H_ */
