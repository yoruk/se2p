#include "Petri_TrafficLight.h"
#include "hw.h"
#include "Mutex.h"
#include "Global.h"



static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petri_TrafficLight* petri; /// the Petri_Bsp object itself

void init_places();
bool trafficlight_places[TRAFFICLIGHT_N_PLACE];

bool trafficlight_lokal_inputs[TRAFFICLIGHT_N_IN];
bool trafficlight_lokal_outputs[N_OUT];

bool* trafficlight_tmpArr;
extern bool notaus;
Petri_TrafficLight::Petri_TrafficLight() {

	trafficlight = TrafficLight::getInstance();
	trafficlightblinken = new TrafficLightBlinken();
	trafficlightgelb = new TrafficLightGelbBlinken();

	trafficlight_dispatcher = Dispatcher::getInstance();
	trafficlight_dispatcher_Chid
			= trafficlight_dispatcher->get_trafficLight_Chid();

}

Petri_TrafficLight::~Petri_TrafficLight() {
	delete petri;
	petri = NULL;
}

// returns the only running instance of Petri_Bsp
Petri_TrafficLight* Petri_TrafficLight::getInstance() {

	mutex->lock();

	if (!init_HW_Done()) {
		init_HW();
	}

	if (!petri) {
		petri = new Petri_TrafficLight();
	}

	mutex->unlock();

	return petri;
}

void Petri_TrafficLight::execute(void* arg) {

	struct _pulse pulse;


	init_places();
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(trafficlight_dispatcher_Chid, &pulse,
				sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("Petri_TrafficLight: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}

		trafficlight_tmpArr
				= trafficlight_dispatcher->get_trafficlight_inputs();
		set_trafficlight_inputs();
		process_transitions();
		calculate_outputs();
		NotifyReactor();

	}
}

void Petri_TrafficLight::init_places() {
	trafficlight_places[0] = true;
	trafficlight_places[1] = false;
	trafficlight_places[2] = false;
	trafficlight_places[3] = false;
	trafficlight_places[4] = false;
}

void Petri_TrafficLight::process_transitions() {
	/*_________T0_________*/
	if (trafficlight_places[0] && !trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_START] == true)
			&& (notaus == false)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = true;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;

		trafficlight_lokal_inputs[TRAFFICLIGHT_START] = false;


	}
	/*_________T1_________*/
	if (!trafficlight_places[0] && trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_YELLOW] == true)
			&& (notaus == false)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = false;
		trafficlight_places[2] = true;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_YELLOW] = false;


	}
	/*_________T2_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] == true)
			&& (notaus == false)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = true;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] = false;


	}

	/*_________T3_________*/
	if (!trafficlight_places[0] && trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_RED] == true) && (notaus
			== false)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = true;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_RED] = false;


	}

	/*_________T4_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& !trafficlight_places[2] && trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] == true)
			&& (notaus == false)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = true;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] = false;


	}

	/*_________T5_________*/
	if (!trafficlight_places[0] && trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_RED_B] == true)
			&& (notaus == false)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = true;
		trafficlight_lokal_inputs[TRAFFICLIGHT_RED_B] = false;

	}
	/*_________T6_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] == true)
			&& (notaus == false)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = true;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] = false;


	}

	/*_________T7_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_RED] == true) && (notaus
			== false)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = true;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_RED] = false;


	}

	/*_________T8_________*/
	if (!trafficlight_places[0] && trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_END] == true) && (notaus
			== false)) {

		trafficlight_places[0] = true;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;

		trafficlight_lokal_inputs[TRAFFICLIGHT_END] = false;
		trafficlight->reset_trafficlight();



	}

	/*_________T9_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_END] == true) && (notaus
			== false)) {

		trafficlight_places[0] = true;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;

		trafficlight_lokal_inputs[TRAFFICLIGHT_END] = false;
		trafficlight->reset_trafficlight();


	}

	/*_________T10_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& !trafficlight_places[2] && trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_END] == true) && (notaus
			== false)) {

		trafficlight_places[0] = true;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;

		trafficlight_lokal_inputs[TRAFFICLIGHT_END] = false;
		trafficlight->reset_trafficlight();


	}

}
void Petri_TrafficLight::calculate_outputs() {

	if (trafficlight_places[0] == true) {
		reset_trafficlight_outputs();
	}

	if (trafficlight_places[1] == true) {

		reset_trafficlight_outputs();
		trafficlight_lokal_outputs[AMPEL_GRUEN] = true;
	}

	if (trafficlight_places[2] == true) {

		reset_trafficlight_outputs();
		trafficlight_lokal_outputs[AMPEL_GELB] = true;
	}

	if (trafficlight_places[3] == true) {

		reset_trafficlight_outputs();
		trafficlight_lokal_outputs[AMPEL_ROT] = true;

	}

	if (trafficlight_places[4] == true) {

		reset_trafficlight_outputs();
		trafficlight_lokal_outputs[AMPEL_ROT_B] = true;
	}
}

void Petri_TrafficLight::write_outputs() {

}
void Petri_TrafficLight::NotifyReactor() {
	if (trafficlight_lokal_outputs[AMPEL_GRUEN] == true) {

		trafficlight->reset_trafficlight();
		trafficlight->greenOn();
	}
	if (trafficlight_lokal_outputs[AMPEL_GELB] == true) {

		trafficlight->reset_trafficlight();
		trafficlight->yellowOn();
	}
	if (trafficlight_lokal_outputs[AMPEL_ROT] == true) {
		trafficlightblinken->stop();
		trafficlightblinken->join();
		trafficlightblinken = new TrafficLightBlinken();
		trafficlight->redOn();
	}
	if (trafficlight_lokal_outputs[AMPEL_ROT_B] == true) {
		trafficlight->reset_trafficlight();
		trafficlightblinken->start(NULL);
	}
}
void Petri_TrafficLight::print_places() {

}

void Petri_TrafficLight::set_trafficlight_inputs() {
	trafficlight_lokal_inputs[TRAFFICLIGHT_START] = trafficlight_tmpArr[0];
	trafficlight_lokal_inputs[TRAFFICLIGHT_YELLOW] = trafficlight_tmpArr[1];
	trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] = trafficlight_tmpArr[2];
	trafficlight_lokal_inputs[TRAFFICLIGHT_RED] = trafficlight_tmpArr[3];
	trafficlight_lokal_inputs[TRAFFICLIGHT_RED_B] = trafficlight_tmpArr[4];
	trafficlight_lokal_inputs[TRAFFICLIGHT_END] = trafficlight_tmpArr[5];
}

void Petri_TrafficLight::set_trafficlight_outputs() {
	trafficlight_lokal_outputs[AMPEL_GRUEN] = trafficlight_tmpArr[5];
	trafficlight_lokal_outputs[AMPEL_GELB] = trafficlight_tmpArr[6];
	trafficlight_lokal_outputs[AMPEL_ROT] = trafficlight_tmpArr[7];
	trafficlight_lokal_outputs[AMPEL_ROT_B] = trafficlight_tmpArr[12];
}

void Petri_TrafficLight::reset_trafficlight_outputs() {
	trafficlight_lokal_outputs[AMPEL_GRUEN] = false;
	trafficlight_lokal_outputs[AMPEL_GELB] = false;
	trafficlight_lokal_outputs[AMPEL_ROT] = false;
	trafficlight_lokal_outputs[AMPEL_ROT_B] = false;
}

void Petri_TrafficLight::reset_trafficlight_inputs() {
	trafficlight_lokal_inputs[TRAFFICLIGHT_START] = false;
	trafficlight_lokal_inputs[TRAFFICLIGHT_YELLOW] = false;
	trafficlight_lokal_inputs[TRAFFICLIGHT_RED] = false;
	trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] = false;
	trafficlight_lokal_inputs[TRAFFICLIGHT_RED] = false;
	trafficlight_lokal_inputs[TRAFFICLIGHT_RED_B] = false;
	trafficlight_lokal_inputs[TRAFFICLIGHT_END] = false;
}
void Petri_TrafficLight::shutdown() {
}

