#include "Petri_Conveyor.h"

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petri_Conveyor* petri; /// the Petri_Bsp object itself

void init_places();
bool conveyor_places[CONVEYOR_N_PLACE];

bool conveyor_lokal_inputs[CONVEYOR_N_IN];
bool conveyor_lokal_outputs[N_OUT];

bool* conveyor_tmpArr;

Petri_Conveyor::Petri_Conveyor() {

	conveyor = Conveyor::getInstance();

	conveyor_dispatcher = Dispatcher::getInstance();
	conveyor_dispatcher_Chid = conveyor_dispatcher->get_conveyor_Chid();

}

Petri_Conveyor::~Petri_Conveyor() {
	delete petri;
	petri = NULL;
}
// returns the only running instance of Petri_Bsp
Petri_Conveyor* Petri_Conveyor::getInstance() {

	mutex->lock();

	if (!init_HW_Done()) {
		init_HW();
	}

	if (!petri) {
		petri = new Petri_Conveyor();
	}

	mutex->unlock();

	return petri;
}

void Petri_Conveyor::execute(void* arg) {

	struct _pulse pulse;

//	printf("Petri_Motor:: conveyor_dispatcher_Chid: %d\n",
//			conveyor_dispatcher_Chid);
//	fflush(stdout);

	init_places();
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(conveyor_dispatcher_Chid, &pulse,
				sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("Petri_Motor: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}

		printf("Petri_Motor:: MesgRecievePulse angekommen! \n");
		fflush(stdout);
		printf("Petri_Motor::    code:%d,  value:%d \n", pulse.code,
				pulse.value.sival_int);

		conveyor_tmpArr = conveyor_dispatcher->get_conveyor_inputs();
		set_conveyor_inputs();

		process_transitions();
		calculate_outputs();
		NotifyReactor();

		conveyor_dispatcher->set_disp_Inputs(conveyor_lokal_inputs);
		conveyor_dispatcher->set_disp_Outputs(conveyor_lokal_outputs);

	}
}

void Petri_Conveyor::init_places() {
	conveyor_places[0] = true;
	conveyor_places[1] = false;
	conveyor_places[2] = false;
	conveyor_places[3] = false;
	conveyor_places[4] = false;
}

void Petri_Conveyor::process_transitions() {




	/*_________T0_________*/
	if (conveyor_places[0] && !conveyor_places[1] && !conveyor_places[2]
			&& !conveyor_places[3] && !conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_START] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = true;
		conveyor_places[2] = false;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		puts("Conveyor: T0\n");fflush(stdout);
	}

	/*_________T1_________*/
	if (!conveyor_places[0] && conveyor_places[1] && !conveyor_places[2]
			&& !conveyor_places[3] && !conveyor_places[4]
			&& ((conveyor_lokal_inputs[P_CONVEYOR_STOP] == true)
					|| conveyor_lokal_inputs[P_CONVEYOR_NOTAUS] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = false;
		conveyor_places[2] = true;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		puts("Conveyor: T1\n");fflush(stdout);
	}

	/*_________T2_________*/
	if (!conveyor_places[0] && !conveyor_places[1] && conveyor_places[2]
			&& !conveyor_places[3] && !conveyor_places[4]
			&& ((conveyor_lokal_inputs[P_CONVEYOR_STOP_X] == true)
					|| conveyor_lokal_inputs[P_CONVEYOR_NOTAUS_X] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = true;
		conveyor_places[2] = false;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		puts("Conveyor: T2\n");fflush(stdout);
	}

	/*_________T3_________*/
	if (!conveyor_places[0] && conveyor_places[1] && !conveyor_places[2]
			&& !conveyor_places[3] && !conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_SLOW] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = false;
		conveyor_places[2] = false;
		conveyor_places[3] = true;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		puts("Conveyor: T3\n");fflush(stdout);
	}

	/*_________T4_________*/
	if (!conveyor_places[0] && !conveyor_places[1] && !conveyor_places[2]
			&& conveyor_places[3] && !conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_SLOW_X] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = true;
		conveyor_places[2] = false;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		puts("Conveyor: T4\n");fflush(stdout);
	}

	/*_________T5_________*/
	if (!conveyor_places[0] && conveyor_places[1] && !conveyor_places[2]
			&& !conveyor_places[3] && !conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_LEFT] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = false;
		conveyor_places[2] = false;
		conveyor_places[3] = false;
		conveyor_places[4] = true;

		reset_conveyor_inputs();
		puts("Conveyor: T5\n");fflush(stdout);
	}

	/*_________T6_________*/
	if (!conveyor_places[0] && !conveyor_places[1] && !conveyor_places[2]
			&& !conveyor_places[3] && conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_RIGHT] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = true;
		conveyor_places[2] = false;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		puts("Conveyor: T6\n");fflush(stdout);
	}

	/*_________T7_________*/
	if (!conveyor_places[0] && !conveyor_places[1] && !conveyor_places[2]
			&& !conveyor_places[3] && conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_STOP] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = false;
		conveyor_places[2] = true;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		puts("Conveyor: T7\n");fflush(stdout);
	}

	/*_________T8_________*/
	if (!conveyor_places[0] && !conveyor_places[1] && !conveyor_places[2]
			&& conveyor_places[3] && !conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_STOP] == true)) {

		conveyor_places[0] = false;
		conveyor_places[1] = false;
		conveyor_places[2] = true;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		puts("Conveyor: T8\n");fflush(stdout);
	}
	/*_________T9_________*/
	if (!conveyor_places[0] && conveyor_places[1] && !conveyor_places[2]
			&& !conveyor_places[3] && !conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_END] == true)) {

		conveyor_places[0] = true;
		conveyor_places[1] = false;
		conveyor_places[2] = false;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		conveyor->resetConveyorBits();
		puts("Conveyor: T8\n");fflush(stdout);
	}
	/*_________T10_________*/
	if (!conveyor_places[0] && !conveyor_places[1] && conveyor_places[2]
			&& !conveyor_places[3] && !conveyor_places[4]
			&& (conveyor_lokal_inputs[P_CONVEYOR_END] == true)) {

		conveyor_places[0] = true;
		conveyor_places[1] = false;
		conveyor_places[2] = false;
		conveyor_places[3] = false;
		conveyor_places[4] = false;

		reset_conveyor_inputs();
		conveyor->resetConveyorBits();
		puts("Conveyor: T8\n");fflush(stdout);
	}
}

void Petri_Conveyor::calculate_outputs() {

	if (conveyor_places[0] == true) {
		reset_conveyor_outputs();
		}

	if (conveyor_places[1] == true) {
		reset_conveyor_outputs();
		conveyor_lokal_outputs[MOTOR_RECHTSLAUF] = true;
	}
	if (conveyor_places[2] == true) {
		reset_conveyor_outputs();
		conveyor_lokal_outputs[MOTOR_STOP] = true;
	}
	if (conveyor_places[3] == true) {
		reset_conveyor_outputs();
		conveyor_lokal_outputs[MOTOR_LANGSAM] = true;
	}
	if (conveyor_places[4] == true) {
		reset_conveyor_outputs();
		conveyor_lokal_outputs[MOTOR_LINKSLAUF] = true;
	}
}

void Petri_Conveyor::write_outputs() {
	// TODO Auto-generated
}

void Petri_Conveyor::NotifyReactor() {

	if (conveyor_lokal_outputs[MOTOR_RECHTSLAUF] == true) {
		conveyor->resetConveyorBits();
		conveyor->moveRight();
	}
	if (conveyor_lokal_outputs[MOTOR_LINKSLAUF] == true) {
		conveyor->resetConveyorBits();
		conveyor->moveLeft();
	}
	if (conveyor_lokal_outputs[MOTOR_LANGSAM] == true) {
		conveyor->resetConveyorBits();
		conveyor->moveRight();
		conveyor->moveSlow();
	}
	if (conveyor_lokal_outputs[MOTOR_STOP] == true) {
		conveyor->resetConveyorBits();
		conveyor->conveyorStop();
	}

}

void Petri_Conveyor::print_places() {
	// TODO Auto-generated
}

void Petri_Conveyor::set_conveyor_inputs() {

	conveyor_lokal_inputs[P_CONVEYOR_START] = conveyor_tmpArr[0];
	conveyor_lokal_inputs[P_CONVEYOR_STOP] = conveyor_tmpArr[1];
	conveyor_lokal_inputs[P_CONVEYOR_STOP_X] = conveyor_tmpArr[2];
	conveyor_lokal_inputs[P_CONVEYOR_SLOW] = conveyor_tmpArr[3];
	conveyor_lokal_inputs[P_CONVEYOR_SLOW_X] = conveyor_tmpArr[4];
	conveyor_lokal_inputs[P_CONVEYOR_RIGHT] = conveyor_tmpArr[5];
	conveyor_lokal_inputs[P_CONVEYOR_LEFT] = conveyor_tmpArr[6];
	conveyor_lokal_inputs[P_CONVEYOR_NOTAUS] = conveyor_tmpArr[7];
	conveyor_lokal_inputs[P_CONVEYOR_NOTAUS_X] = conveyor_tmpArr[8];
	conveyor_lokal_inputs[P_CONVEYOR_END] = conveyor_tmpArr[9];
}

void Petri_Conveyor::set_conveyor_outputs() {

	conveyor_lokal_outputs[MOTOR_RECHTSLAUF] = conveyor_tmpArr[0];
	conveyor_lokal_outputs[MOTOR_LINKSLAUF] = conveyor_tmpArr[1];
	conveyor_lokal_outputs[MOTOR_LANGSAM] = conveyor_tmpArr[2];
	conveyor_lokal_outputs[MOTOR_STOP] = conveyor_tmpArr[3];
//	conveyor_lokal_outputs[WEICHE_AUF] = conveyor_tmpArr[4];
//	conveyor_lokal_outputs[AMPEL_GRUEN] = conveyor_tmpArr[5];
//	conveyor_lokal_outputs[AMPEL_GELB] = conveyor_tmpArr[6];
//	conveyor_lokal_outputs[AMPEL_ROT] = conveyor_tmpArr[7];
//	conveyor_lokal_outputs[LED_STARTTASTE] = conveyor_tmpArr[8];
//	conveyor_lokal_outputs[LED_RESETTASTE] = conveyor_tmpArr[9];
//	conveyor_lokal_outputs[LED_Q1] = conveyor_tmpArr[10];
//	conveyor_lokal_outputs[LED_Q2] = conveyor_tmpArr[11];
//	conveyor_lokal_outputs[AMPEL_ROT_B] = conveyor_tmpArr[12];
}

void Petri_Conveyor::reset_conveyor_inputs(){

	conveyor_lokal_inputs[P_CONVEYOR_START] = false;
	conveyor_lokal_inputs[P_CONVEYOR_STOP] = false;
	conveyor_lokal_inputs[P_CONVEYOR_STOP_X] = false;
	conveyor_lokal_inputs[P_CONVEYOR_SLOW] = false;
	conveyor_lokal_inputs[P_CONVEYOR_SLOW_X] = false;
	conveyor_lokal_inputs[P_CONVEYOR_RIGHT] = false;
	conveyor_lokal_inputs[P_CONVEYOR_LEFT] = false;
	conveyor_lokal_inputs[P_CONVEYOR_NOTAUS] = false;
	conveyor_lokal_inputs[P_CONVEYOR_NOTAUS_X] = false;
}


void Petri_Conveyor::reset_conveyor_outputs(){

	conveyor_lokal_outputs[MOTOR_RECHTSLAUF] = false;
	conveyor_lokal_outputs[MOTOR_LINKSLAUF] = false;
	conveyor_lokal_outputs[MOTOR_LANGSAM] = false;
	conveyor_lokal_outputs[MOTOR_STOP] = false;
}
void Petri_Conveyor::shutdown() {
}
