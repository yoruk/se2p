#include "Petri_Controller_2.h"

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petri_Controller_2* petri; /// the Petri_Controller_2 object itself

void init_places();
bool first_chance = true;
bool* tmpArr;
bool places[N_PLACE_2];
bool petri_controller_2_inputs[N_IN];
bool petri_controller_2_outputs[N_OUT];
bool gate_close_c2_timeout;
bool rutsche_voll_c2_timeout;
bool aussortieren;

Petri_Controller_2::Petri_Controller_2() {

	petri_controller_2_sen = Sensorik::getInstance();
	petri_controller_2_sensorik_Chid = petri_controller_2_sen->getSignalChid();
	//	printf("petri_controller_2_:: BLAU: Coid %d\n",
	//			petri_controller_2_dispatcher_Coid);
	//	fflush(stdout);
	//	printf("petri_controller_2_:: BLAU: Chid %d\n",
	//			petri_controller_2_sensorik_Chid);
	//	fflush(stdout);

	TimerHandler* timer = TimerHandler::getInstance();
	timer_C2_GateClose = timer->createTimer(petri_controller_2_sensorik_Chid, C2_CLOSE_GATE_TIME, 0, TIMER_GATE);

	timer_C2_SlideFull = timer->createTimer(petri_controller_2_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

	// attach to signal channel
	petri_controller_2_dispatcher_Coid = ConnectAttach(0, 0, petri_controller_2_sensorik_Chid, _NTO_SIDE_CHANNEL, 0);
	if (petri_controller_2_dispatcher_Coid == -1) {
		perror("petri_controller_2_: ConnectAttach dispatcher_Coid failed");
		exit(EXIT_FAILURE);
	}

	gate = Gate::getInstance();
	led = Led::getInstance();

	disp_petri_controller_2 = Dispatcher::getInstance();
	petri_controller_2_dispatcher_Chid = disp_petri_controller_2->get_disp_Chid();

	puk_c2.set_typ(PUK_GROSS);
}

Petri_Controller_2::~Petri_Controller_2() {
	delete petri;
	petri = NULL;
}

// returns the only running instance of Petri_Bsp
Petri_Controller_2* Petri_Controller_2::getInstance() {

	mutex->lock();

	if (!init_HW_Done()) {
		init_HW();
	}

	if (!petri) {
		petri = new Petri_Controller_2();
	}

	mutex->unlock();

	return petri;
}

void Petri_Controller_2::execute(void* arg) {

	struct _pulse pulse;

	//	printf("Petri_Controller_2:: GLEB: Chid %d\n",
	//			petri_controller_2_dispatcher_Chid);
	//	fflush(stdout);

	init_places();
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(petri_controller_2_dispatcher_Chid, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("petri_controller_2_dispatcher_Chid: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}

		//printf("petri_controller_2::MesgRecievePulse\n");fflush(stdout);

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_GATE) {
			gate_close_c2_timeout = true;
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_FULL) {
			rutsche_voll_c2_timeout = true;
		}

		tmpArr = disp_petri_controller_2->get_disp_Inputs();
		setInputs();
		tmpArr = disp_petri_controller_2->get_disp_Outputs();
		setOutputs();

		process_transitions();
		calculate_outputs();
		NotifyReactor();

		disp_petri_controller_2->set_disp_Inputs(petri_controller_2_inputs);
		disp_petri_controller_2->set_disp_Outputs(petri_controller_2_outputs);
	}
}

void Petri_Controller_2::init_places() {

	places[0] = true;
	places[1] = false;
	places[2] = false;
	places[3] = false;
	places[4] = false;
	places[5] = false;
	places[6] = false;
	places[7] = false;
	places[8] = false;
	places[9] = false;
	places[10] = false;
	places[11] = false;
	places[12] = false;
	places[13] = false;
	places[14] = false;
	places[15] = false;
	places[16] = false;
	places[17] = false;
	places[18] = false;

}

void Petri_Controller_2::process_transitions() {
	/*_________T0_________*/
	if (places[0] && !places[1] && (petri_controller_2_inputs[EINLAUF_WERKSTUECK] == true)) {

		places[0] = false;
		places[1] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_START)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}
		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_START)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}
		//petri_controller_2_inputs[EINLAUF_WERKSTUECK] = false;
		puts("Petri_Controller_2:  T0\n");
		fflush(stdout);

	}

	/*_________T1_________*/
	if (places[1] && !places[2] && (petri_controller_2_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == true)) {

		places[1] = false;
		places[2] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		//		if (puk_c2.get_typ() == PUK_LOCH) {
		//			puk_c2.set_hoehenmessung2(puk_c2.get_hoehenmessung1());
		//		}
		//
		//		if (puk_c2.get_typ() == PUK_METALL) {
		//			puk_c2.set_hoehenmessung2(puk_c2.get_hoehenmessung1());
		//		}

		puk_tmp_type = petri_controller_2_sen->getHeightPukType();

		if (puk_c2.get_typ() == PUK_GROSS && puk_tmp_type == PUK_GROSS) {
			aussortieren = true;
		}

		puts("Petri_Controller_2:  T1\n");
		fflush(stdout);
	}

	/*_________T2_________*/
	if (places[2] && !places[3] && (petri_controller_2_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == false)) {

		places[2] = false;
		places[3] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW_X)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}
		puts("Petri_Controller_2:  T2\n");
		fflush(stdout);
	}
	/*_________T3_________*/
	if (places[3] && !places[4] && (petri_controller_2_inputs[WERKSTUECK_IN_WEICHE] == true)) {

		places[3] = false;
		places[4] = true;

		puts("Petri_Controller_2:  T3\n");
		fflush(stdout);
		//		petri_controller_2_inputs[WERKSTUECK_IN_WEICHE] = false;
	}

	/*_________T4_________*/
	if (places[4] && !places[5] && (petri_controller_2_inputs[WERKSTUECK_METALL] == false) && aussortieren == false) {

		places[4] = false;
		places[5] = true;

		gate->open();
		timer_C2_GateClose->start();
		//		petri_controller_2_outputs[WEICHE_AUF] = true;
		puts("Petri_Controller_2:  T4\n");
		fflush(stdout);
	}
	/*_________T5_________*/
	if (places[5] && !places[6] && gate_close_c2_timeout == true) {
		gate->close();
		places[5] = false;
		places[6] = true;

		//petri_controller_2_outputs[WEICHE_AUF] = false;  && gate_close_c2_timeout == true
		timer_C2_GateClose->reset();
		gate_close_c2_timeout = false;
		puts("Petri_Controller_2:  T5\n");
		fflush(stdout);
	}
	/*_________T6_________*/
	if (places[6] && !places[7] && (petri_controller_2_inputs[AUSLAUF_WERKSTUECK] == true)) {

		places[6] = false;
		places[7] = true;
		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_YELLOW)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		//petri_controller_2_outputs[WEICHE_AUF] = false;
		puts("Petri_Controller_2:  T6\n");
		fflush(stdout);
	}

	/*_________T7_________*/
	if (places[7] && !places[0] && (petri_controller_2_inputs[AUSLAUF_WERKSTUECK] == false)) {

		places[7] = false;
		places[0] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_END)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}
		puts("Petri_Controller_2:  T7\n");
		fflush(stdout);

		first_chance = true;
	}

	/*_________T8_________*/
	if (places[4] && !places[8] && (petri_controller_2_inputs[WERKSTUECK_METALL] == true)) {

		places[4] = false;
		places[8] = true;

		puk_c2.set_typ(PUK_METALL);

		//petri_controller_2_inputs[WERKSTUECK_METALL] = false;
		puts("Petri_Controller_2:  T8\n");
		fflush(stdout);
	}

	if (first_chance == true) {

		/*_________T9_________*/
		if (places[8] && !places[9]) {

			places[8] = false;
			places[9] = true;

			if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_LEFT)) {
				perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
				puts("Petri_Controller_2:  T9\n");
				fflush(stdout);
			}

			first_chance = false;
		}
	}

	/*_________T10_________*/
	if (places[9] && !places[10] && (petri_controller_2_inputs[EINLAUF_WERKSTUECK] == true)) {

		places[9] = false;
		places[10] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}
		puts("Petri_Controller_2:  T10\n");
		fflush(stdout);

	}

	/*_________T11_________*/
	if (places[10] && !places[11] && (petri_controller_2_inputs[EINLAUF_WERKSTUECK] == false)) {

		places[10] = false;
		places[11] = true;

		puts("Petri_Controller_2:  T11\n");
		fflush(stdout);
	}

	/*_________T12_________*/
	if (places[11] && !places[1] && (petri_controller_2_inputs[EINLAUF_WERKSTUECK] == true)) {

		places[11] = false;
		places[1] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP_X)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}
		//petri_controller_2_inputs[EINLAUF_WERKSTUECK] == false;
		puts("Petri_Controller_2:  T12\n");
		fflush(stdout);
	}

	if (first_chance == false) {
		/*_________T13_________*/
		if (places[8] && !places[12]) {

			places[8] = false;
			places[12] = true;

			puts("Petri_Controller_2:  T13\n");
			fflush(stdout);
			first_chance = true;
		}

	}

	/*_________T14_________*/
	if (places[12] && !places[13] && (petri_controller_2_inputs[RUTSCHE_VOLL] == true)) {

		places[12] = false;
		places[13] = true;

		timer_C2_SlideFull->start();

		puts("Petri_Controller_2:  T14\n");
		fflush(stdout);
	}

	/*_________T15_________*/
	if (places[13] && !places[14] && (petri_controller_2_inputs[RUTSCHE_VOLL] == false)) {

		places[13] = false;
		places[14] = true;

		puts("Petri_Controller_2:  T15\n");
		fflush(stdout);
	}

	/*_________T17_________*/
	if (places[13] && !places[15] && rutsche_voll_c2_timeout == true) {

		places[13] = false;
		places[15] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED_B)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		timer_C2_SlideFull->reset();
		rutsche_voll_c2_timeout = false;

		puts("Petri_Controller_2:  T17\n");

	}

	/*_________T18_________*/
	if (places[15] && !places[16] && (petri_controller_2_inputs[TASTE_RESET] == true)) {

		places[15] = false;
		places[16] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_2:  T18\n");

	}

	/*_________T19_________*/
	if (places[16] && !places[14] && (petri_controller_2_inputs[RUTSCHE_VOLL] == false)) {

		places[16] = false;
		places[14] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP_X)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_GREEN)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_2:  T19\n");
	}

	/*_________T20_________*/
	if (places[4] && !places[12] && (petri_controller_2_inputs[WERKSTUECK_METALL] == false) && aussortieren == true) {

		places[4] = false;
		places[12] = true;

		//		timer_C2_GateClose->start();
		printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<HIER WIRD GROSS AUSSORTIERT!!!");
		puts("Petri_Controller_2:  T4\n");
		fflush(stdout);

		aussortieren = false;
	}

	/*_________T16_________*/
	if (places[14] && !places[0]) {

		places[0] = true;
		places[14] = false;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_END)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}
		puts("Petri_Controller_2:  T16\n");
		fflush(stdout);
	}
}

void Petri_Controller_2::calculate_outputs() {
	if (places[0]) {
		petri_controller_2_outputs[WEICHE_AUF] = false;
		petri_controller_2_outputs[LED_STARTTASTE] = false;
		petri_controller_2_outputs[LED_RESETTASTE] = false;
		petri_controller_2_outputs[LED_Q1] = false;
		petri_controller_2_outputs[LED_Q2] = false;
	}
}

void Petri_Controller_2::NotifyReactor() {

	if (petri_controller_2_outputs[WEICHE_AUF]) {

		gate->open();
	} else {
		//		gate->close();
	}

	if (petri_controller_2_outputs[LED_STARTTASTE]) {
		led->led_StartButton_On();
	} else {
		led->led_StartButton_Off();
	}
	if (petri_controller_2_outputs[LED_RESETTASTE]) {
		led->led_ResetButton_On();
	} else {
		led->led_ResetButton_Off();
	}
	if (petri_controller_2_outputs[LED_Q1]) {
		led->led_Q1_On();
	} else {
		led->led_Q1_Off();
	}
	if (petri_controller_2_outputs[LED_Q2]) {
		led->led_Q2_On();
	} else {
		led->led_Q2_Off();
	}
}

void Petri_Controller_2::setInputs() {
	petri_controller_2_inputs[EINLAUF_WERKSTUECK] = tmpArr[EINLAUF_WERKSTUECK];
	petri_controller_2_inputs[WERKSTUECK_IN_HOEHENMESSUNG] = tmpArr[WERKSTUECK_IN_HOEHENMESSUNG];
	petri_controller_2_inputs[HOENMESSUNG] = tmpArr[HOENMESSUNG];
	petri_controller_2_inputs[WERKSTUECK_IN_WEICHE] = tmpArr[WERKSTUECK_IN_WEICHE];
	petri_controller_2_inputs[WERKSTUECK_METALL] = tmpArr[WERKSTUECK_METALL];
	petri_controller_2_inputs[WEICHE_OFFEN] = tmpArr[WEICHE_OFFEN];
	petri_controller_2_inputs[RUTSCHE_VOLL] = tmpArr[RUTSCHE_VOLL];
	petri_controller_2_inputs[AUSLAUF_WERKSTUECK] = tmpArr[AUSLAUF_WERKSTUECK];
	petri_controller_2_inputs[TASTE_START] = tmpArr[TASTE_START];
	petri_controller_2_inputs[TASTE_STOP] = tmpArr[TASTE_STOP];
	petri_controller_2_inputs[TASTE_RESET] = tmpArr[TASTE_RESET];
	petri_controller_2_inputs[TASTE_E_STOP] = tmpArr[TASTE_E_STOP];
}
void Petri_Controller_2::setOutputs() {
	//	petri_controller_2_outputs[MOTOR_RECHTSLAUF] = tmpArr[0];
	//	petri_controller_2_outputs[MOTOR_LINKSLAUF] = tmpArr[1];
	//	petri_controller_2_outputs[MOTOR_LANGSAM] = tmpArr[2];
	//	petri_controller_2_outputs[MOTOR_STOP] = tmpArr[3];
	petri_controller_2_outputs[WEICHE_AUF] = tmpArr[4];
	//	petri_controller_2_outputs[AMPEL_GRUEN] = tmpArr[5];
	//	petri_controller_2_outputs[AMPEL_GELB] = tmpArr[6];
	//	petri_controller_2_outputs[AMPEL_ROT] = tmpArr[7];
	petri_controller_2_outputs[LED_STARTTASTE] = tmpArr[8];
	petri_controller_2_outputs[LED_RESETTASTE] = tmpArr[9];
	petri_controller_2_outputs[LED_Q1] = tmpArr[10];
	petri_controller_2_outputs[LED_Q2] = tmpArr[11];
	//	petri_controller_2_outputs[AMPEL_ROT_B] = tmpArr[12];
}

void Petri_Controller_2::shutdown() {

}
