#include "Petri_TrafficLight.h"

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petri_TrafficLight* petri; /// the Petri_Bsp object itself

void init_places();
bool trafficlight_places[TRAFFICLIGHT_N_PLACE];

bool trafficlight_lokal_inputs[TRAFFICLIGHT_N_IN];
bool trafficlight_lokal_outputs[N_OUT];

bool* trafficlight_tmpArr;

Petri_TrafficLight::Petri_TrafficLight() {

	trafficlight = TrafficLight::getInstance();

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

	printf("Petri_TrafficLight:: trafficlight_dispatcher_Chid: %d\n",
			trafficlight_dispatcher_Chid);
	fflush(stdout);

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

		printf("Petri_TrafficLight:: MesgRecievePulse angekommen! \n");
		fflush(stdout);
		printf("Petri_TrafficLight::    code:%d,  value:%d \n", pulse.code,
				pulse.value.sival_int);

		trafficlight_tmpArr
				= trafficlight_dispatcher->get_trafficlight_inputs();
		set_trafficlight_inputs();

		process_transitions();
		calculate_outputs();
		NotifyReactor();

		trafficlight_dispatcher->set_disp_Inputs(trafficlight_lokal_inputs);

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
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_START] == true)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = true;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;

		trafficlight_lokal_inputs[TRAFFICLIGHT_START] = false;
		puts("Trafficlight: T0\n");
		fflush(stdout);

	}
	/*_________T1_________*/
	if (!trafficlight_places[0] && trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_YELLOW] == true)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = false;
		trafficlight_places[2] = true;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_YELLOW] = false;
		puts("Trafficlight: T1\n");
		fflush(stdout);

	}
	/*_________T2_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] == true)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = true;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] = false;
		puts("Trafficlight: T2\n");
		fflush(stdout);

	}

	/*_________T3_________*/
	if (!trafficlight_places[0] && trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_RED] == true)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = true;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_RED] = false;
		puts("Trafficlight: T3\n");
		fflush(stdout);

	}

	/*_________T4_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& !trafficlight_places[2] && trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] == true)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = true;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] = false;
		puts("Trafficlight: T4\n");
		fflush(stdout);

	}

	/*_________T5_________*/
	if (!trafficlight_places[0] && trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_RED_B] == true)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = true;
		trafficlight_lokal_inputs[TRAFFICLIGHT_RED_B] = false;
		puts("Trafficlight: T5\n");
		fflush(stdout);
	}
	/*_________T6_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] == true)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = true;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_GREEN] = false;
		puts("Trafficlight: T6\n");
		fflush(stdout);

	}

	/*_________T7_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_RED] == true)) {

		trafficlight_places[0] = false;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = true;
		trafficlight_places[4] = false;
		trafficlight_lokal_inputs[TRAFFICLIGHT_RED] = false;
		puts("Trafficlight: T7\n");
		fflush(stdout);

	}

	/*_________T8_________*/
	if (!trafficlight_places[0] && trafficlight_places[1]
			&& !trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_END] == true)) {

		trafficlight_places[0] = true;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;

		trafficlight_lokal_inputs[TRAFFICLIGHT_END] = false;
		trafficlight->reset_trafficlight();
		puts("Trafficlight: T7\n");
		fflush(stdout);

	}

	/*_________T9_________*/
	if (!trafficlight_places[0] && !trafficlight_places[1]
			&& trafficlight_places[2] && !trafficlight_places[3]
			&& !trafficlight_places[4]
			&& (trafficlight_lokal_inputs[TRAFFICLIGHT_END] == true)) {

		trafficlight_places[0] = true;
		trafficlight_places[1] = false;
		trafficlight_places[2] = false;
		trafficlight_places[3] = false;
		trafficlight_places[4] = false;

		trafficlight_lokal_inputs[TRAFFICLIGHT_END] = false;
		trafficlight->reset_trafficlight();
		puts("Trafficlight: T7\n");
		fflush(stdout);

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
	// TODO Auto-generated
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

		trafficlight->reset_trafficlight();
		trafficlight->redOn();
	}
	if (trafficlight_lokal_outputs[AMPEL_ROT_B] == true) {

		trafficlight->reset_trafficlight();
		trafficlight->flashRedOn();
	}
}
void Petri_TrafficLight::print_places() {
	// TODO Auto-generated
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
	// TODO Auto-generated
	//	trafficlight_lokal_outputs[MOTOR_RECHTSLAUF] = trafficlight_tmpArr[0];
	//	trafficlight_lokal_outputs[MOTOR_LINKSLAUF] = trafficlight_tmpArr[1];
	//	trafficlight_lokal_outputs[MOTOR_LANGSAM] = trafficlight_tmpArr[2];
	//	trafficlight_lokal_outputs[MOTOR_STOP] = trafficlight_tmpArr[3];
	//	trafficlight_lokal_outputs[WEICHE_AUF] = trafficlight_tmpArr[4];
	trafficlight_lokal_outputs[AMPEL_GRUEN] = trafficlight_tmpArr[5];
	trafficlight_lokal_outputs[AMPEL_GELB] = trafficlight_tmpArr[6];
	trafficlight_lokal_outputs[AMPEL_ROT] = trafficlight_tmpArr[7];
	//	trafficlight_lokal_outputs[LED_STARTTASTE] = trafficlight_tmpArr[8];
	//	trafficlight_lokal_outputs[LED_RESETTASTE] = trafficlight_tmpArr[9];
	//	trafficlight_lokal_outputs[LED_Q1] = trafficlight_tmpArr[10];
	//	trafficlight_lokal_outputs[LED_Q2] = trafficlight_tmpArr[11];
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

