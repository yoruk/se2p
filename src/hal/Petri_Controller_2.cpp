#include "Petri_Controller_2.h"
#include "Puk.h"
#include "SerialCom.h"
#include <unistd.h>

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petri_Controller_2* petri; /// the Petri_Controller_2 object itself

static bool first_chance = true;
static bool* tmpArr;
static bool places[N_PLACE_2];
static bool petri_controller_2_inputs[N_IN];
static bool petri_controller_2_outputs[N_OUT];
static bool gate_close_c2_timeout;
static bool rutsche_voll_c2_timeout;
static bool Puk_removed_c2_timeout;
static bool Puk_added_LSH_c2_timeout;
static bool Puk_added_LSW_c2_timeout;
static bool Puk_added_LSEND_c2_timeout;

static bool aussortieren;
static SerialCom* sc;
extern bool notaus;

Petri_Controller_2::Petri_Controller_2() {

	petri_controller_2_sen = Sensorik::getInstance();
	petri_controller_2_sensorik_Chid = petri_controller_2_sen->getSignalChid();
	//	printf("petri_controller_2_:: BLAU: Coid %d\n",
	//			petri_controller_2_dispatcher_Coid);
	//	fflush(stdout);
	//	printf("petri_controller_2_:: BLAU: Chid %d\n",
	//			petri_controller_2_sensorik_Chid);
	//	fflush(stdout);

	timer_c2 = TimerHandler::getInstance();
	timer_C2_GateClose = timer_c2->createTimer(petri_controller_2_sensorik_Chid, C2_CLOSE_GATE_TIME, 0, TIMER_GATE);

	timer_C2_SlideFull = timer_c2->createTimer(petri_controller_2_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

	timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

	timer_C2_To_added_LSH = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 800, TIMER_ADDED_LSH);

	timer_C2_To_added_LSW = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 0, 500, TIMER_ADDED_LSW);

	timer_C2_To_added_LSEND = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 0, TIMER_ADDED_LSEND);

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

	//puk_c2.set_typ(PUK_GROSS);

	sc = SerialCom::getInstance();
}

Petri_Controller_2::~Petri_Controller_2() {
	delete petri;
	petri = NULL;

	delete sc;
	sc = NULL;
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

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_REMOVED) {
			Puk_removed_c2_timeout = true;
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_ADDED_LSH) {
			Puk_added_LSH_c2_timeout = true;
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_ADDED_LSW) {
			Puk_added_LSW_c2_timeout = true;
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_ADDED_LSEND) {
			Puk_added_LSEND_c2_timeout = true;
		}

		if (pulse.code == PULSE_PUK_INFORMATION) {
			puk_c2 = *(Puk::intToPuk(pulse.value.sival_int));
		}

		tmpArr = disp_petri_controller_2->get_disp_Inputs();
		setInputs();
		tmpArr = disp_petri_controller_2->get_disp_Outputs();
		//setOutputs();

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
	places[19] = false;
	places[20] = false;
}

void Petri_Controller_2::process_transitions() {
	/*_________T0_________*/
	if (places[0] && !places[1] && (petri_controller_2_inputs[EINLAUF_WERKSTUECK] == true) && (notaus ==false)) {

		places[0] = false;
		places[1] = true;

		sc->send_msg_pkg(CONTROLLER_2_BUSY);

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

		timer_C2_To_removed->start();
		timer_C2_To_added_LSH->start();
	}

	/*_________T1 _________*/
	if (places[1] && !places[2] && (petri_controller_2_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == true) && (Puk_added_LSH_c2_timeout == true)&& (notaus ==false)) {

		places[1] = false;
		places[2] = true;

		Puk_added_LSH_c2_timeout = false;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		puk_tmp_type = petri_controller_2_sen->getHeightPukType();

		if ((puk_c2.get_typ() == PUK_GROSS) && (puk_tmp_type == PUK_GROSS)) {
			aussortieren = true;
		} else {
			puk_c2.set_hoehenmessung2(petri_controller_2_sen->getHeight());
			puk_c2.set_typ(petri_controller_2_sen->getHeightPukType());
		}

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSH);
		timer_C2_To_added_LSH = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 800, TIMER_ADDED_LSH);

		printf("<<<<<BOOL :  %s  \n", aussortieren ? "true" : "false");
		fflush(stdout);

		puts("Petri_Controller_2:  T1\n");
		fflush(stdout);
	}

	/*_________T2_________*/
	if (places[2] && !places[3] && (petri_controller_2_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == false)&& (notaus ==false)) {

		places[2] = false;
		places[3] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW_X)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		timer_C2_To_removed->start();
		timer_C2_To_added_LSW->start();

		puts("Petri_Controller_2:  T2\n");
		fflush(stdout);
	}
	/*_________T3_________*/
	if (places[3] && !places[4] && (petri_controller_2_inputs[WERKSTUECK_IN_WEICHE] == true) && (Puk_added_LSW_c2_timeout == true)&& (notaus ==false)) {

		Puk_added_LSW_c2_timeout = false;

		places[3] = false;
		places[4] = true;

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSW);
		timer_C2_To_added_LSW = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 0, 500, TIMER_ADDED_LSW);


		//timeout Puk verschwindet für LSEND
		timer_C2_To_removed->start();



		puts("Petri_Controller_2:  T3\n");
		fflush(stdout);
		//		petri_controller_2_inputs[WERKSTUECK_IN_WEICHE] = false;
	}

	/*_________T4_________*/
	if (places[4] && !places[5] && (petri_controller_2_inputs[WERKSTUECK_METALL] == false) && aussortieren == false&& (notaus ==false)) {

		places[4] = false;
		places[5] = true;

		//gate->open();
		timer_C2_GateClose->start();
		petri_controller_2_outputs[WEICHE_AUF] = true;
		puts("Petri_Controller_2:  T4\n");
		fflush(stdout);
	}
	/*_________T5_________*/
	if (places[5] && !places[6] && gate_close_c2_timeout == true&& (notaus ==false)) {

		places[5] = false;
		places[6] = true;

		petri_controller_2_outputs[WEICHE_AUF] = false;
		timer_C2_GateClose->reset();
		timer_C2_To_added_LSEND->start();
		gate_close_c2_timeout = false;
		puts("Petri_Controller_2:  T5\n");
		fflush(stdout);
	}
	/*_________T6_________*/
	if (places[6] && !places[7] && (petri_controller_2_inputs[AUSLAUF_WERKSTUECK] == true) && (Puk_added_LSEND_c2_timeout == true)&& (notaus ==false)) {

		Puk_added_LSEND_c2_timeout = false;

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

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSEND);
		timer_C2_To_added_LSEND = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 0, TIMER_ADDED_LSEND);

		puts("Petri_Controller_2:  T6\n");
		fflush(stdout);
	}

	/*_________T7_________*/
	if (places[7] && !places[0] && (petri_controller_2_inputs[AUSLAUF_WERKSTUECK] == false)&& (notaus ==false)) {

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

		sc->send_msg_pkg(CONTROLLER_2_FREE);

		puts("Petri_Controller_2:  T7\n");
		fflush(stdout);

		first_chance = true;
	}

	//ToDo Metal erkennung + Second Chance
	/*_________T8_________*/
	if (places[4] && !places[8] && (petri_controller_2_inputs[WERKSTUECK_METALL] == true)&& (notaus ==false)) {

		places[4] = false;
		places[8] = true;

		puk_c2.set_typ(PUK_METALL);

		//petri_controller_2_inputs[WERKSTUECK_METALL] = false;
		puts("Petri_Controller_2:  T8\n");
		fflush(stdout);
	}

	if (first_chance == true) {

		/*_________T9_________*/
		if (places[8] && !places[9]&& (notaus ==false)) {

			places[8] = false;
			places[9] = true;

			if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_LEFT)) {
				perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
				puts("Petri_Controller_2:  T9\n");
				fflush(stdout);
			}

			timer_c2->deleteTimer(timer_C2_To_removed);
			timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

			timer_c2->deleteTimer(timer_C2_To_added_LSW);
			timer_C2_To_added_LSW = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 0, 500, TIMER_ADDED_LSW);
			timer_c2->deleteTimer(timer_C2_To_added_LSH);
			timer_C2_To_added_LSH = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 800, TIMER_ADDED_LSH);

			first_chance = false;
		}
	}

	/*_________T10_________*/
	if (places[9] && !places[10] && (petri_controller_2_inputs[EINLAUF_WERKSTUECK] == true)&& (notaus ==false)) {

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
	if (places[10] && !places[11] && (petri_controller_2_inputs[EINLAUF_WERKSTUECK] == false)&& (notaus ==false)) {

		places[10] = false;
		places[11] = true;

		puts("Petri_Controller_2:  T11\n");
		fflush(stdout);
	}

	/*_________T12_________*/
	if (places[11] && !places[1] && (petri_controller_2_inputs[EINLAUF_WERKSTUECK] == true)&& (notaus ==false)) {

		places[11] = false;
		places[1] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP_X)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}
		//petri_controller_2_inputs[EINLAUF_WERKSTUECK] == false;

		timer_C2_To_added_LSH->start();
		puts("Petri_Controller_2:  T12\n");
		fflush(stdout);
	}

	if (first_chance == false) {
		/*_________T13_________*/
		if (places[8] && !places[12]&& (notaus ==false)) {

			places[8] = false;
			places[12] = true;

			gate->open();
			usleep(50000);
			gate->close();
			puts("Petri_Controller_2:  T13\n");
			fflush(stdout);
			first_chance = true;
		}

	}

	//ToDo Rutsche
	/*_________T14_________*/
	if (places[12] && !places[13] && (petri_controller_2_inputs[RUTSCHE_VOLL] == true) &&(notaus ==false)) {

		places[12] = false;
		places[13] = true;

		timer_C2_SlideFull->start();

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);
		puts("Petri_Controller_2:  T14\n");
		fflush(stdout);
	}

	/*_________T15_________*/
	if (places[13] && !places[14] && (petri_controller_2_inputs[RUTSCHE_VOLL] == false)&&(notaus ==false)) {

		places[13] = false;
		places[14] = true;

		timer_c2->deleteTimer(timer_C2_SlideFull);
		timer_C2_SlideFull = timer_c2->createTimer(petri_controller_2_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

		puts("Petri_Controller_2:  T15\n");
		fflush(stdout);
	}

	/*_________T17_________*/
	if (places[13] && !places[15] && rutsche_voll_c2_timeout == true&&(notaus ==false)) {

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

		rutsche_voll_c2_timeout = false;
		timer_c2->deleteTimer(timer_C2_SlideFull);
		timer_C2_SlideFull = timer_c2->createTimer(petri_controller_2_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

		puts("Petri_Controller_2:  T17\n");

	}

	/*_________T18_________*/
	if (places[15] && !places[16] && (petri_controller_2_inputs[TASTE_RESET] == true)&&(notaus ==false)) {

		places[15] = false;
		places[16] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_2:  T18\n");

	}

	/*_________T19_________*/
	if (places[16] && !places[17] && (petri_controller_2_inputs[RUTSCHE_VOLL] == false)&&(notaus ==false)) {

		places[16] = false;
		places[17] = true;

		puts("Petri_Controller_2:  T19\n");
	}

	/*_________T20_________*/
	if (places[17] && !places[14] && (petri_controller_2_inputs[TASTE_START] == true)&&(notaus ==false)) {

		places[17] = false;
		places[14] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP_X)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_GREEN)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		sc->send_msg_pkg(CONTROLLER_2_FREE);
		puts("Petri_Controller_2:  T20 \n");

	}

	/*_________T21_________*/
	if (places[4] && !places[12] && (petri_controller_2_inputs[WERKSTUECK_METALL] == false) && aussortieren == true) {

		places[4] = false;
		places[12] = true;

		gate->open();
		usleep(50000);
		gate->close();

		//		timer_C2_GateClose->start();
		printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<HIER WIRD GROSS AUSSORTIERT!!!");
		puts("Petri_Controller_2:  T21 \n");
		fflush(stdout);

		aussortieren = false;
	}

	//ToDo  Zurück zum Anfang
	/*_________T16_________*/
	if (places[14] && !places[0]&&(notaus ==false)) {

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

		sc->send_msg_pkg(CONTROLLER_2_FREE);

		puts("Petri_Controller_2:  T16\n");
		fflush(stdout);
	}

	//ToDO  Puk Verschwindet to LSH

	/*_________T22_________*/
	if (places[1] && !places[18] && (Puk_removed_c2_timeout == true)&&(notaus ==false)) {

		Puk_removed_c2_timeout = false;

		Puk_added_LSH_c2_timeout = false;


		places[1] = false;
		places[18] = true;

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSH);
		timer_C2_To_added_LSH = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 800, TIMER_ADDED_LSH);



		puts("Petri_Controller_2:  T22\n");
		fflush(stdout);
	}

	//ToDO  Puk Verschwindet to LSW
	/*_________T26_________*/
	if (places[3] && !places[18] && (Puk_removed_c2_timeout == true)&&(notaus ==false)) {

		Puk_removed_c2_timeout = false;
		Puk_added_LSW_c2_timeout = false;


		places[3] = false;
		places[18] = true;

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSW);
		timer_C2_To_added_LSW = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 0, 500, TIMER_ADDED_LSW);

		puts("Petri_Controller_2:  T26\n");
		fflush(stdout);
	}

	//ToDO  Puk Verschwindet to LSEND

	/*_________T27_________*/
	if (places[6] && !places[18] && (Puk_removed_c2_timeout == true)&&(notaus ==false)) {

		Puk_removed_c2_timeout = false;
		Puk_added_LSEND_c2_timeout = false;

		places[6] = false;
		places[18] = true;

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSEND);
		timer_C2_To_added_LSEND = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 0, TIMER_ADDED_LSEND);


		puts("Petri_Controller_2:  T27 \n");
		fflush(stdout);
	}

	//ToDO Puk wurde hinzugefügt LSH
	/*_________T28_________*/
	if (places[1] && !places[18] && (petri_controller_2_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == true) && (Puk_added_LSH_c2_timeout == false) && (first_chance == true)&&(notaus ==false)) {

		places[1] = false;
		places[18] = true;

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSH);
		timer_C2_To_added_LSH = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 800, TIMER_ADDED_LSH);

		puts("Petri_Controller_2:  T28 \n");
		fflush(stdout);
	}

	//ToDo PUK Hinzugefügt LSW
	/*_________T29_________*/
	if (places[3] && !places[18] && (petri_controller_2_inputs[WERKSTUECK_IN_WEICHE] == true) && (Puk_added_LSW_c2_timeout == false)&&(notaus ==false)) {

		places[3] = false;
		places[18] = true;

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSW);
		timer_C2_To_added_LSW = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 0, 500, TIMER_ADDED_LSW);

		puts("Petri_Controller_2:  T29 \n");
		fflush(stdout);
		//		petri_controller_2_inputs[WERKSTUECK_IN_WEICHE] = false;
	}

	//ToDo PUK Hinzugefügt LSEND
	/*_________T30_________*/
	if (places[6] && !places[18] && (petri_controller_2_inputs[AUSLAUF_WERKSTUECK] == true) && (Puk_added_LSEND_c2_timeout == false)&&(notaus ==false)) {

		places[6] = false;
		places[18] = true;

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSEND);
		timer_C2_To_added_LSEND = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 0, TIMER_ADDED_LSEND);

		puts("Petri_Controller_2:  T30\n");
		fflush(stdout);
		//		petri_controller_2_inputs[WERKSTUECK_IN_WEICHE] = false;
	}

	/*_________T23_________*/
	if (places[18] && !places[19]&&(notaus ==false)) {

		places[18] = false;
		places[19] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED_B)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_2:  T23 \n");
		fflush(stdout);

		aussortieren = false;
	}

	/*_________T24_________*/
	if (places[19] && !places[20] && (petri_controller_2_inputs[TASTE_RESET] == true)&&(notaus ==false)) {

		places[19] = false;
		places[20] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_2:  T24\n");

	}

	/*_________T25_________*/
	if (places[20] && !places[0] && (petri_controller_2_inputs[TASTE_START] == true)&&(notaus ==false)) {

		places[20] = false;
		places[0] = true;

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
			perror("Petri_Controller_2:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_2_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_END)) {
			perror("Petri_Controller_2:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		timer_c2->deleteTimer(timer_C2_To_removed);
		timer_C2_To_removed = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 4, 0, TIMER_REMOVED);

		timer_c2->deleteTimer(timer_C2_To_added_LSH);
		timer_C2_To_added_LSH = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 800, TIMER_ADDED_LSH);

		timer_c2->deleteTimer(timer_C2_To_added_LSW);
		timer_C2_To_added_LSW = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 0, 500, TIMER_ADDED_LSW);

		timer_c2->deleteTimer(timer_C2_To_added_LSEND);
		timer_C2_To_added_LSEND = timer_c2->createTimer(petri_controller_2_sensorik_Chid, 1, 0, TIMER_ADDED_LSEND);

		sc->send_msg_pkg(CONTROLLER_2_FREE);

		puts("Petri_Controller_2:  T25 \n");

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
		gate->close();
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
