#include "Petri_Controller_1.h"
#include <unistd.h>

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petri_Controller_1* petri; /// the Petri_Controller_1 object itself

void init_places();
bool* tmpArr_1;
int p[N_PLACE];
bool werkstueckGross;
bool petri_controller_1_inputs[N_IN];
bool petri_controller_1_outputs[N_OUT];
bool gate_close_c1_timeout;


Petri_Controller_1::Petri_Controller_1() {

	petri_controller_1_sen = Sensorik::getInstance();
	petri_controller_1_sensorik_Chid = petri_controller_1_sen->getSignalChid();
	printf("petri_controller_1_:: BLAU: Coid %d\n",
			petri_controller_1_dispatcher_Coid);
	fflush(stdout);
	printf("petri_controller_1_:: BLAU: Chid %d\n",
			petri_controller_1_sensorik_Chid);
	fflush(stdout);

	TimerHandler* timer = TimerHandler::getInstance();
	timer1 = timer->createTimer(petri_controller_1_sensorik_Chid, 4, 0, TIMER_GATE);

	// attach to signal channel
	petri_controller_1_dispatcher_Coid = ConnectAttach(0, 0,
			petri_controller_1_sensorik_Chid, _NTO_SIDE_CHANNEL, 0);
	if (petri_controller_1_dispatcher_Coid == -1) {
		perror("petri_controller_1_: ConnectAttach dispatcher_Coid failed");
		exit(EXIT_FAILURE);
	}

	gate1 = Gate::getInstance();

	led = Led::getInstance();

	disp_petri_controller_1 = Dispatcher::getInstance();
	petri_controller_1_dispatcher_Chid
			= disp_petri_controller_1->get_disp_Chid();



}

Petri_Controller_1::~Petri_Controller_1() {
	delete petri;
	petri = NULL;
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

	printf("Petri_Controller_1:: GLEB: Chid %d\n",
			petri_controller_1_dispatcher_Chid);
	fflush(stdout);

	init_places();
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(petri_controller_1_dispatcher_Chid, &pulse,
				sizeof(pulse), NULL)) {
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

		tmpArr_1 = disp_petri_controller_1->get_disp_Inputs();
		setInputs();
		tmpArr_1 = disp_petri_controller_1->get_disp_Outputs();
		setOutputs();
		process_transitions();
		calculate_outputs();
		NotifyReactor();

		disp_petri_controller_1->set_disp_Inputs(petri_controller_1_inputs);
		disp_petri_controller_1->set_disp_Outputs(petri_controller_1_outputs);
	}
}

void Petri_Controller_1::init_places() {

	p[0] = 4;
	p[1] = 0;
	p[2] = 0;
	p[3] = 0;
	p[4] = 0;
	p[5] = 0;
	p[6] = 0;
	p[7] = 0;
	p[8] = 0;
	p[9] = 0;
	p[10] = 0;
	p[11] = 0;
	p[12] = 0;
	p[13] = 0;
	p[14] = 0;
	p[15] = 0;
	p[16] = 0;
	p[17] = 0;
	p[18] = 0;
	p[19] = 0;
	p[20] = 0;
	p[21] = 0;
	p[22] = 0;

}

void Petri_Controller_1::process_transitions() {
	/*_________T0_________*/
	if (p[0] != 0 && p[1] == 0 && (petri_controller_1_inputs[EINLAUF_WERKSTUECK] == true)) {
		p[0]--;
		p[1] = 1;

		fifo1.put(puk);

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
				SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_START)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}
		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
				SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_START)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_1:  T0		moving to H (1)\n"); fflush(stdout);
	}

	/*_________T1_________*/
	if (p[1] == 1 && p[2] == 0) {
		p[1] = 0;
		p[2] = 1;

		puts("Petri_Controller_1:  T1		moving to H (2)\n");
		fflush(stdout);
	}

	/*_________T2_________*/
	if (p[2] == 1 && p[3] == 0) {

		p[2] = 0;
		p[3] = 1;

		puts("Petri_Controller_1:  T2		moving to H (3)\n");
		fflush(stdout);
	}

	/*_________T3_________*/
	if (p[3] == 1 && p[4] == 0) {

		p[3] = 0;
		p[4] = 1;

		puts("Petri_Controller_1:  T3		moving to H (4)\n");
		fflush(stdout);
	}

	/*_________T4_________*/
	if (p[4] == 1 && p[5] == 0 && (petri_controller_1_inputs[HOENMESSUNG]== true)) {


		werkstueckGross = true;

		puts("Petri_Controller_1:  T4\n");
		fflush(stdout);
	}

	/*_________T5_________*/
	if (p[4] == 1 && p[5] == 0
			&& (petri_controller_1_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == true)) {

		p[4] = 0;
		p[5] = 1;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
				SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}
		fifo1.get()->set_hoehenmessung1(petri_controller_1_sen->getHeight());
		fifo1.get()->set_typ(petri_controller_1_sen->getHeightPuckType());
		fifo2.put(fifo1.remove());

		puts("Petri_Controller_1:  T5	\n");
		fflush(stdout);
	}

	/*_________T6_________*/
	if (p[5] == 1 && p[6] == 0
			&& (petri_controller_1_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == false)) {

		p[5] = 0;
		p[6] = 1;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
				SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW_X)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_1:  T6	 \n");
		fflush(stdout);
	}

	/*_________T7_________*/
	if (p[6] == 1 && p[7] == 0) {

		p[6] = 0;
		p[7] = 1;

		puts("Petri_Controller_1:  T7 		moving to W (4)\n");
		fflush(stdout);
	}

	/*_________T8_________*/
	if (p[7] == 1 && p[8] == 0) {

		p[7] = 0;
		p[8] = 1;

		puts("Petri_Controller_1:  T8		moving to W (4)\n");
		fflush(stdout);
	}

	/*_________T9_________*/
	if (p[8] == 1 && p[9] == 0) {

		p[8] = 0;
		p[9] = 1;

		puts("Petri_Controller_1:  T9		moving to W (4)\n");
		fflush(stdout);
	}

	/*_________T10_________*/
	if (p[9] == 1 && p[10] == 0
			&& (petri_controller_1_inputs[WERKSTUECK_IN_WEICHE] == true)) {

		p[9] = 0;
		p[10] = 1;

		puts("Petri_Controller_1:  T10		\n");
		fflush(stdout);

	}

	/*_________T11_________*/
	if (p[10] == 1 && p[11] == 0
			&& (petri_controller_1_inputs[WERKSTUECK_METALL] == true)) {


		fifo2.get()->set_typ(PUK_METALL);
		fifo3.put(fifo2.remove());

		puts("Petri_Controller_1:  T11		\n");
		fflush(stdout);

	}

	/*_________T12_________*/
	if (p[10] == 1 && p[11] == 0 && werkstueckGross == true) {

		p[10] = 0;
		p[11] = 1;

		gate1->open();
		petri_controller_1_outputs[WEICHE_AUF] = true;

		fifo3.put(fifo2.remove());

		timer1->start();
		werkstueckGross=false;
		puts("Petri_Controller_1:  T12	 (4)\n");
		fflush(stdout);
	}

	/*_________T13_________*/
	if (p[11] == 1 && p[12] == 0 && gate_close_c1_timeout == true) {

		gate_close_c1_timeout = false;

		p[11] = 0;
		p[12] = 1;

		petri_controller_1_outputs[WEICHE_AUF] = false;
		timer1->reset();
		puts("Petri_Controller_1:  T13		 (4)\n");
		fflush(stdout);
	}

	/*_________T14_________*/
	if (p[12] == 1 && p[13] == 0) {

		p[12] = 0;
		p[13] = 1;

		puts("Petri_Controller_1:  T14		moving to W (4)\n");
		fflush(stdout);
	}

	/*_________T15_________*/
	if (p[13] == 1 && p[14] == 0) {

		p[13] = 0;
		p[14] = 1;

		puts("Petri_Controller_1:  T15		moving to W (4)\n");
		fflush(stdout);
	}

	/*_________T16_________*/
	if (p[14] == 1 && p[15] == 0) {

		p[14] = 0;
		p[15] = 1;

		puts("Petri_Controller_1:  T16		moving to W (4)\n");
		fflush(stdout);
	}

	/*_________T17_________*/
	if (p[15] == 1 && p[16] == 0
			&& (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == true)) {

		p[15] = 0;
		p[16] = 1;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
				SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_YELLOW)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}
		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
				SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		puts("Petri_Controller_1:  T17		 \n");
		fflush(stdout);
	}

	/*_________T18_________*/
	if (p[16] == 1 && (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == false)) {

		p[16] = 0;

		p[0]++;

		//HIER ÜBERGABE PUK AN STRUCT -----> STRUCT VIA SERIELLESCHNITTSTELLE AN BAND 2

		if (p[0] == 4) {
			if (-1== MsgSendPulse(petri_controller_1_dispatcher_Coid,
							SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT,
							TRAFFICLIGHT_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
				exit(EXIT_FAILURE);
			}
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
					SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
			}
		} else {

			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
					SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT,
					TRAFFICLIGHT_GREEN)) {
				perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
				exit(EXIT_FAILURE);
			}
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid,
					SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP_X)) {
				perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
			}
		}
		puts("Petri_Controller_1:  T18		 \n");
		fflush(stdout);
	}

	/*_________T18_________*/
	if (p[10] == 1 && p[17] == 0 && werkstueckGross == false) {

		p[10] = 0;
		p[17] = 1;


		puts("Petri_Controller_1:  T18	 \n");
		fflush(stdout);
	}

	/*_________T19_________*/
	if (p[17] == 1 && p[18] == 0 && (petri_controller_1_inputs[RUTSCHE_VOLL] == true) ) {

		p[17] = 0;
		p[18] = 1;


		puts("Petri_Controller_1:  T19	 \n");
		fflush(stdout);
	}

	/*_________T20_________*/
	if (p[18] == 1 && p[19] == 0 && (petri_controller_1_inputs[RUTSCHE_VOLL] == false) ) {

		p[18] = 0;
		p[19] = 1;


		puts("Petri_Controller_1:  T20	 \n");
		fflush(stdout);
	}

	/*_________T21_________*/
	if (p[19] == 1 && p[20] == 0) {

		p[19] = 0;
		p[20] = 1;

		puts("Petri_Controller_1:  T21		moving from Slide to GZ  (1)\n");
		fflush(stdout);
	}

	/*_________T22_________*/
	if (p[20] == 1 && p[21] == 0) {

		p[20] = 0;
		p[21] = 1;

		puts("Petri_Controller_1:  T22		moving from Slide to GZ  (2)\n");
		fflush(stdout);
	}

	/*_________T23_________*/
	if (p[21] == 1 && p[22] == 0) {
		printf("ANZAHL:   %d",p[0]);fflush(stdout);

		p[21] = 0;
		p[22] = 1;

		printf("ANZAHL:   %d",p[0]);fflush(stdout);
		puts("Petri_Controller_1:  T23		moving from Slide to GZ  (3)\n");
		fflush(stdout);
	}

	/*_________T24_________*/
	if (p[22] == 1) {
		printf("ANZAHL:   %d",p[0]);fflush(stdout);

		p[22] = 0;
		p[0]++;

		printf("ANZAHL:   %d",p[0]);fflush(stdout);
		puts("Petri_Controller_1:  T24		moving from Slide to GZ (4)\n");
		fflush(stdout);
	}
}

void Petri_Controller_1::calculate_outputs() {
}

void Petri_Controller_1::NotifyReactor() {
	printf("WEICHE %s", petri_controller_1_outputs[WEICHE_AUF] ? "true"
			: "false");
	fflush(stdout);
	if (petri_controller_1_outputs[WEICHE_AUF] == true) {
		puts("HIERRR OPEN.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		fflush(stdout);
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
	petri_controller_1_inputs[EINLAUF_WERKSTUECK]
			= tmpArr_1[EINLAUF_WERKSTUECK];
	petri_controller_1_inputs[WERKSTUECK_IN_HOEHENMESSUNG]
			= tmpArr_1[WERKSTUECK_IN_HOEHENMESSUNG];
	petri_controller_1_inputs[HOENMESSUNG] = tmpArr_1[HOENMESSUNG];
	petri_controller_1_inputs[WERKSTUECK_IN_WEICHE]
			= tmpArr_1[WERKSTUECK_IN_WEICHE];
	petri_controller_1_inputs[WERKSTUECK_METALL] = tmpArr_1[WERKSTUECK_METALL];
	petri_controller_1_inputs[WEICHE_OFFEN] = tmpArr_1[WEICHE_OFFEN];
	petri_controller_1_inputs[RUTSCHE_VOLL] = tmpArr_1[RUTSCHE_VOLL];
	petri_controller_1_inputs[AUSLAUF_WERKSTUECK]
			= tmpArr_1[AUSLAUF_WERKSTUECK];
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

