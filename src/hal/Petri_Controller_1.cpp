#include "Petri_Controller_1.h"
#include <unistd.h>
#include "SerialCom.h"

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petri_Controller_1* petri; /// the Petri_Controller_1 object itself

// beispiel bandende
/*
 * sc->send_puk_data_pkg(int puk_id, int puk_type, int puk_height)
 * sc->send_msg_pkg(int msg_content)
 *
 * receivePulse()...
 *
 * wenn: status = siehe value
 * wenn: puk = Puk::intToPuk(value)
 */

static void init_places();
static bool* tmpArr_1;
static Puk* p[N_PLACE];

static bool petri_controller_1_inputs[N_IN];
static bool petri_controller_1_outputs[N_OUT];

static int gz;
static bool gate_close_c1_timeout;
static bool rutsche_voll_c1_timeout;
static bool newPuk = true;
static bool test;

static SerialCom* sc;

Petri_Controller_1::Petri_Controller_1() {

	petri_controller_1_sen = Sensorik::getInstance();
	petri_controller_1_sensorik_Chid = petri_controller_1_sen->getSignalChid();
	//	printf("petri_controller_1_:: BLAU: Coid %d\n",
	//			petri_controller_1_dispatcher_Coid);
	//	fflush(stdout);
	//	printf("petri_controller_1_:: BLAU: Chid %d\n",
	//			petri_controller_1_sensorik_Chid);
	//	fflush(stdout);

	TimerHandler* timer = TimerHandler::getInstance();
	timer_Gate = timer->createTimer(petri_controller_1_sensorik_Chid, 0, C1_CLOSE_GATE_TIME, TIMER_GATE);

	timer_C1_SlideFull = timer->createTimer(petri_controller_1_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

	// attach to signal channel
	petri_controller_1_dispatcher_Coid = ConnectAttach(0, 0, petri_controller_1_sensorik_Chid, _NTO_SIDE_CHANNEL, 0);
	if (petri_controller_1_dispatcher_Coid == -1) {
		perror("petri_controller_1_: ConnectAttach dispatcher_Coid failed");
		exit(EXIT_FAILURE);
	}

	gate1 = Gate::getInstance();

	led = Led::getInstance();

	disp_petri_controller_1 = Dispatcher::getInstance();
	petri_controller_1_dispatcher_Chid = disp_petri_controller_1->get_disp_Chid();

	sc = SerialCom::getInstance();
}

Petri_Controller_1::~Petri_Controller_1() {
	delete petri;
	petri = NULL;

	delete sc;
	sc = NULL;
}

// returns the only running instance of Petri_Bsp
Petri_Controller_1* Petri_Controller_1::getInstance() {

	mutex->lock();

	if (!init_HW_Done()) {
		init_HW();
	}

	if (!petri) {
		petri = new Petri_Controller_1();
	}

	mutex->unlock();

	return petri;
}

void Petri_Controller_1::execute(void* arg) {

	struct _pulse pulse;

	//	printf("Petri_Controller_1:: GLEB: Chid %d\n",
	//			petri_controller_1_dispatcher_Chid);fflush(stdout);

	init_places();
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(petri_controller_1_dispatcher_Chid, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("petri_controller_1_dispatcher_Chid: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}

		//printf("petri_controller_2::MesgRecievePulse\n");fflush(stdout);

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_GATE) {
			gate_close_c1_timeout = true;
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_FULL) {
			rutsche_voll_c1_timeout = true;
		}

		tmpArr_1 = disp_petri_controller_1->get_disp_Inputs();
		setInputs();
		tmpArr_1 = disp_petri_controller_1->get_disp_Outputs();

		//setOutputs();
		process_transitions();
		calculate_outputs();
		NotifyReactor();

		disp_petri_controller_1->set_disp_Inputs(petri_controller_1_inputs);
		disp_petri_controller_1->set_disp_Outputs(petri_controller_1_outputs);
	}
}

void Petri_Controller_1::init_places() {
	gz = 4;
	p[0] = NULL;
	p[1] = NULL;
	p[2] = NULL;
	p[3] = NULL;
	p[4] = NULL;
	p[5] = NULL;
	p[6] = NULL;
	p[7] = NULL;
	p[8] = NULL;
	p[9] = NULL;
	p[10] = NULL;
	p[11] = NULL;
	p[12] = NULL;
	p[13] = NULL;
	p[14] = NULL;
	p[15] = NULL;
	p[16] = NULL;
	p[17] = NULL;
	p[18] = NULL;
	p[19] = NULL;
	p[20] = NULL;
	p[21] = NULL;
	p[22] = NULL;
	p[23] = NULL;
	p[24] = NULL;
	p[25] = NULL;
	p[26] = NULL;
	p[27] = NULL;
	p[28] = NULL;
	p[29] = NULL;
	p[30] = NULL;
	p[31] = NULL;
	p[32] = NULL;

}

void Petri_Controller_1::process_transitions() {

	/*_________T0_________*/
	if ((gz > 0) && p[0] == NULL && (petri_controller_1_inputs[EINLAUF_WERKSTUECK] == true) && newPuk == true) {

		gz--;

		Puk* puk1 = new Puk();

		p[0] = puk1;
		newPuk = false;
		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_START)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}
		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_START)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_1:  T0	moving to Werkstueck Hoehenmessung \n");
		fflush(stdout);
	}

	/*_________T1_________*/
	if (p[0] != NULL && p[1] == NULL && (petri_controller_1_inputs[EINLAUF_WERKSTUECK] == false)) {
		//petri_controller_1_outputs[WEICHE_AUF]= true;
		p[1] = p[0];
		p[0] = NULL;

		newPuk = true;
		puts("Petri_Controller_1:  T1 \n");
		fflush(stdout);
	}

	/*_________T2_________*/
	if (p[1] != NULL && p[2] == NULL) {
		p[2] = p[1];
		p[1] = NULL;

		puts("Petri_Controller_1:  T2		moving to Werkstueck Hoehenmessung (2)\n");
		fflush(stdout);
	}

	/*_________T3_________*/
	if (p[2] != NULL && p[3] == NULL) {

		p[3] = p[2];
		p[2] = NULL;

		puts("Petri_Controller_1:  T3		moving to Werkstueck Hoehenmessung (3)\n");
		fflush(stdout);
	}

	/*_________T4_________*/
	if (p[3] != NULL && p[4] == NULL) {

		p[4] = p[3];
		p[3] = NULL;

		puts("Petri_Controller_1:  T4		moving to Werkstueck Hoehenmessung (4)\n");
		fflush(stdout);
	}

	/*_________T5_________*/
	if (p[4] != NULL && p[5] == NULL && (petri_controller_1_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == true)) {

		p[5] = p[4];
		p[4] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		p[5]->set_hoehenmessung1(petri_controller_1_sen->getHeight());
		p[5]->set_typ(petri_controller_1_sen->getHeightPukType());

		//			p[5]->set_typ(PUK_FLACH);
	}

	/*_________T6_________*/
	if (p[5] != NULL && p[6] == NULL && (petri_controller_1_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == false)) {

		p[6] = p[5];
		p[5] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW_X)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_1:  T6	 LS H x  (1)\n");
		fflush(stdout);
	}

	/*_________T7_________*/
	if (p[6] != NULL && p[7] == NULL) {

		p[7] = p[6];
		p[6] = NULL;

		puts("Petri_Controller_1:  T7		moving to LS W (2)\n");
		fflush(stdout);
	}

	/*_________T8________*/
	if (p[7] != NULL && p[8] == NULL) {

		p[8] = p[7];
		p[7] = NULL;

		puts("Petri_Controller_1:  T8		moving to LS W (3) \n");
		fflush(stdout);
	}

	/*_________T9_________*/
	if (p[8] != NULL && p[9] == NULL) {

		p[9] = p[8];
		p[8] = NULL;

		puts("Petri_Controller_1:  T9		moving to LS W (4) \n");
		fflush(stdout);
	}

	/*_________T10_________*/
	if (p[9] != NULL && p[10] == NULL && (petri_controller_1_inputs[WERKSTUECK_IN_WEICHE] == true)) {

		p[10] = p[9];
		p[9] = NULL;

		puts("Petri_Controller_1:  T10		moving to LS W \n");
		fflush(stdout);
	}

	/*_________T11_________*/
	if (p[10] != NULL && p[11] == NULL && (petri_controller_1_inputs[WERKSTUECK_METALL] == true)) {

		p[11] = p[10];
		p[10] = NULL;

		p[11]->set_typ(PUK_METALL);

		puts("Petri_Controller_1:  T11		METAL \n");
		fflush(stdout);
	}

	/*_________T12_________*/
	if (p[10] != NULL && p[11] == NULL && (petri_controller_1_inputs[WERKSTUECK_METALL] == false)) {

		p[11] = p[10];
		p[10] = NULL;

		puts("Petri_Controller_1:  T12		METAL x \n");
		fflush(stdout);
	}

	/*_________T13_________*/
	if (p[11] != NULL && p[12] == NULL && (p[11]->get_typ() == PUK_GROSS || p[11]->get_typ() == PUK_LOCH || p[11]->get_typ() == PUK_METALL)) {

		p[12] = p[11];
		p[11] = NULL;

		//gate1->open();
		petri_controller_1_outputs[WEICHE_AUF] = true;

		puts("Petri_Controller_1:  T13		KEINE KLEINE WERKSTUECKE \n");
		fflush(stdout);
	}

	/*_________T14_________*/
	if (p[12] != NULL && p[13] == NULL && (petri_controller_1_inputs[TASTE_E_STOP] == false)) {

		p[13] = p[12];
		p[12] = NULL;

		puts("Petri_Controller_1:  T14		NAus x \n");
		fflush(stdout);
	}

	/*_________T15_________*/
	if (p[13] != NULL && p[14] == NULL && (petri_controller_1_inputs[WERKSTUECK_IN_WEICHE] == false)) {

		p[14] = p[13];
		p[13] = NULL;

		timer_Gate->start();

		puts("Petri_Controller_1:  T15		Weiche x \n");
		fflush(stdout);
	}

	/*_________T16_________*/
	if (p[14] != NULL && p[15] == NULL && gate_close_c1_timeout == true) {
		//if (p[14] != NULL && p[15] == NULL && gate_close_c1_timeout == true) {

		gate_close_c1_timeout = false;

		p[15] = p[14];
		p[14] = NULL;

		//		int i;
		//		Puk* tmp = NULL;
		//		for (i = 13; i >= 0; i--) {
		//			if (p[i] != NULL) {
		//				tmp = p[i];
		//				break;
		//			}
		//		}
		//
		//		if (tmp != NULL) {
		//			if (tmp->get_typ() == PUK_FLACH) {
		//				petri_controller_1_outputs[WEICHE_AUF] = false;
		//			}
		//		} else {
		//			petri_controller_1_outputs[WEICHE_AUF] = false;
		//		}
		petri_controller_1_outputs[WEICHE_AUF] = false;
		timer_Gate->reset();
		puts("Petri_Controller_1:  T16		 (4)\n");
		fflush(stdout);
	}

	/*_________T17_________*/
	if (p[15] != NULL && p[16] == NULL) {

		p[16] = p[15];
		p[15] = NULL;

		puts("Petri_Controller_1:  T17		moving to END (2)\n");
		fflush(stdout);
	}

	/*_________T18_________*/
	if (p[16] != NULL && p[17] == NULL) {

		p[17] = p[16];
		p[16] = NULL;

		puts("Petri_Controller_1:  T18		moving to END (3)\n");
		fflush(stdout);
	}

	/*_________T19_________*/
	if (p[17] != NULL && p[18] == NULL) {

		p[18] = p[17];
		p[17] = NULL;

		//		if (timer_started == true) {
		//			timer_Gate->pause();
		//		}

		puts("Petri_Controller_1:  T19		moving to END (4)\n");
		fflush(stdout);
	}

	/*_________T20_________*/
	if (p[18] != NULL && p[19] == NULL && (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == true)) {

		p[19] = p[18];
		p[18] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_YELLOW)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_1:  T20		 (4)\n");
		fflush(stdout);
	}

	/*_________T21_________*/
	if (p[19] != NULL && (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == false)) {

		gz++;

		//TO DO hier puk weiter geben an BAND 2!!!
		sc->send_puk_data_pkg(p[19]->get_id(), p[19]->get_typ(), p[19]->get_hoehenmessung1());

		p[19] = NULL;

		if (gz == 4) {
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
				exit(EXIT_FAILURE);
			}
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
			}
		} else {

			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_GREEN)) {
				perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
				exit(EXIT_FAILURE);
			}
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP_X)) {
				perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
			}
		}

		puts("Petri_Controller_1:  T21		 (4)\n");
		fflush(stdout);

	}

	/*_________T22_________*/
	if (p[11] != NULL && p[20] == NULL && p[11]->get_typ() == PUK_FLACH) {

		p[20] = p[11];
		p[11] = NULL;

		gate1->open();
		usleep(75000);
		gate1->close();

		puts("Petri_Controller_1:  T22		WERKST�CK IST FLACH\n");
		fflush(stdout);
	}

	/*_________T23_________*/
	if (p[20] != NULL && p[21] == NULL && (petri_controller_1_inputs[RUTSCHE_VOLL] == true)) {

		p[21] = p[20];
		p[20] = NULL;

		timer_C1_SlideFull->start();

		puts("Petri_Controller_1:  T23		moving to LS-RUTSCHE \n");
		fflush(stdout);
	}

	/*_________T24_________*/
	if (p[21] != NULL && p[22] == NULL && (petri_controller_1_inputs[RUTSCHE_VOLL] == false)) {

		p[22] = p[21];
		p[21] = NULL;

		if (gz == 3) {
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
			}

			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
			}
		}

		timer_C1_SlideFull->reset();

		puts("Petri_Controller_1:  T24		leaving LS-RUTSCHE \n");
		fflush(stdout);
	}

	/*_________T25_________*/
	if (p[22] != NULL && p[23] == NULL) {

		p[23] = p[22];
		p[22] = NULL;

		puts("Petri_Controller_1:  T25	 \n");
		fflush(stdout);
	}

	/*_________T26_________*/
	if (p[23] != NULL && p[24] == NULL) {

		p[24] = p[23];
		p[23] = NULL;

		puts("Petri_Controller_1:  T26	 \n");
		fflush(stdout);
	}

	/*_________T27_________*/
	if (p[24] != NULL && p[25] == NULL) {

		p[25] = p[24];
		p[24] = NULL;

		puts("Petri_Controller_1:  T27	 \n");
		fflush(stdout);
	}

	/*_________T28_________*/
	if (p[25] != NULL) {

		gz++;

		//TO DO hier puk weiter geben an BAND 2!!!

		p[25] = NULL;

		puts("Petri_Controller_1:  T28 to GZ \n");
		fflush(stdout);
	}

}

void Petri_Controller_1::calculate_outputs() {
}

void Petri_Controller_1::NotifyReactor() {

	if (petri_controller_1_outputs[WEICHE_AUF] == true) {
		//puts("HIERRR OPEN.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");fflush(stdout);
		gate1->open();
	} else {

		gate1->close();
	}

	if (petri_controller_1_outputs[LED_STARTTASTE]) {
		led->led_StartButton_On();
	} else {
		led->led_StartButton_Off();
	}
	if (petri_controller_1_outputs[LED_RESETTASTE]) {
		led->led_ResetButton_On();
	} else {
		led->led_ResetButton_Off();
	}
	if (petri_controller_1_outputs[LED_Q1]) {
		led->led_Q1_On();
	} else {
		led->led_Q1_Off();
	}
	if (petri_controller_1_outputs[LED_Q2]) {
		led->led_Q2_On();
	} else {
		led->led_Q2_Off();
	}
}

void Petri_Controller_1::setInputs() {
	petri_controller_1_inputs[EINLAUF_WERKSTUECK] = tmpArr_1[EINLAUF_WERKSTUECK];
	petri_controller_1_inputs[WERKSTUECK_IN_HOEHENMESSUNG] = tmpArr_1[WERKSTUECK_IN_HOEHENMESSUNG];
	petri_controller_1_inputs[HOENMESSUNG] = tmpArr_1[HOENMESSUNG];
	petri_controller_1_inputs[WERKSTUECK_IN_WEICHE] = tmpArr_1[WERKSTUECK_IN_WEICHE];
	petri_controller_1_inputs[WERKSTUECK_METALL] = tmpArr_1[WERKSTUECK_METALL];
	petri_controller_1_inputs[WEICHE_OFFEN] = tmpArr_1[WEICHE_OFFEN];
	petri_controller_1_inputs[RUTSCHE_VOLL] = tmpArr_1[RUTSCHE_VOLL];
	petri_controller_1_inputs[AUSLAUF_WERKSTUECK] = tmpArr_1[AUSLAUF_WERKSTUECK];
	petri_controller_1_inputs[TASTE_START] = tmpArr_1[TASTE_START];
	petri_controller_1_inputs[TASTE_STOP] = tmpArr_1[TASTE_STOP];
	petri_controller_1_inputs[TASTE_RESET] = tmpArr_1[TASTE_RESET];
	petri_controller_1_inputs[TASTE_E_STOP] = tmpArr_1[TASTE_E_STOP];
}
void Petri_Controller_1::setOutputs() {
	//	petri_controller_1_outputs[MOTOR_RECHTSLAUF] = tmpArr[0];
	//	petri_controller_1_outputs[MOTOR_LINKSLAUF] = tmpArr[1];
	//	petri_controller_1_outputs[MOTOR_LANGSAM] = tmpArr[2];
	//	petri_controller_1_outputs[MOTOR_STOP] = tmpArr[3];
	petri_controller_1_outputs[WEICHE_AUF] = tmpArr_1[4];

	//	petri_controller_1_outputs[AMPEL_GRUEN] = tmpArr[5];
	//	petri_controller_1_outputs[AMPEL_GELB] = tmpArr[6];
	//	petri_controller_1_outputs[AMPEL_ROT] = tmpArr[7];
	petri_controller_1_outputs[LED_STARTTASTE] = tmpArr_1[8];
	petri_controller_1_outputs[LED_RESETTASTE] = tmpArr_1[9];
	petri_controller_1_outputs[LED_Q1] = tmpArr_1[10];
	petri_controller_1_outputs[LED_Q2] = tmpArr_1[11];
	//	petri_controller_1_outputs[AMPEL_ROT_B] = tmpArr[12];
}

void Petri_Controller_1::shutdown() {

}

