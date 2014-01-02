#include "Petri_Controller_1.h"
#include <unistd.h>
#include "SerialCom.h"

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petri_Controller_1* petri; /// the Petri_Controller_1 object itself

void init_places();
bool* tmpArr_1;
Puk* p[N_PLACE];

bool petri_controller_1_inputs[N_IN];
bool petri_controller_1_outputs[N_OUT];

int gz;
bool gate_close_c1_timeout;
bool rutsche_voll_c1_timeout;
bool newPuk = true;
bool test;
bool fehler = false;
static SerialCom* sc;
bool move_to_c1_timeout;
bool removed_C1_timeout_LSH;
bool removed_C1_timeout_LSW;
bool removed_C1_timeout_LSEND;
bool end;
extern bool notaus;
Petri_Controller_1::Petri_Controller_1() {

	petri_controller_1_sen = Sensorik::getInstance();
	petri_controller_1_sensorik_Chid = petri_controller_1_sen->getSignalChid();
	//	printf("petri_controller_1_:: BLAU: Coid %d\n",
	//			petri_controller_1_dispatcher_Coid);
	//	fflush(stdout);
	//	printf("petri_controller_1_:: BLAU: Chid %d\n",
	//			petri_controller_1_sensorik_Chid);
	//	fflush(stdout);

	timer_c1 = TimerHandler::getInstance();
	timer_Gate = timer_c1->createTimer(petri_controller_1_sensorik_Chid, 0, C1_CLOSE_GATE_TIME, TIMER_GATE);

	timer_C1_SlideFull = timer_c1->createTimer(petri_controller_1_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

	timer_move = timer_c1->createTimer(petri_controller_1_sensorik_Chid, 0, C1_MOVE_TO_TIME, TIMER_MOVE);

	timer_C1_removed_LSH = timer_c1->createTimer(petri_controller_1_sensorik_Chid, 4,0, TIMER_REMOVED_C1_LSH);


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

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_MOVE) {
			move_to_c1_timeout = true;
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_REMOVED_C1_LSH) {
			removed_C1_timeout_LSH = true;
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_REMOVED_C1_LSW) {
			removed_C1_timeout_LSW = true;
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == TIMER_REMOVED_C1_LSEND) {
			removed_C1_timeout_LSEND = true;
		}
		tmpArr_1 = disp_petri_controller_1->get_disp_Inputs();
		setInputs();



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
	p[33] = NULL;
	p[34] = NULL;
	p[35] = NULL;
	p[36] = NULL;
	p[37] = NULL;
	p[38] = NULL;

}

void Petri_Controller_1::process_transitions() {

	/*_________T0_________*/
	if ((gz > 0) && p[0] == NULL && (petri_controller_1_inputs[EINLAUF_WERKSTUECK] == true) && newPuk == true && fehler == false && (notaus == false)) {

		gz--;

		printf("GZ : %d\n", gz);
		fflush(stdout);

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
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T0	EINLAUF_WERKSTUECK  \n", p[0]->get_id(), p[0]->get_hoehenmessung1(), p[0]->get_typ());
		fflush(stdout);


	}

	/*_________T1_________*/
	if (p[0] != NULL && p[1] == NULL && (petri_controller_1_inputs[EINLAUF_WERKSTUECK] == false) && (fehler == false) && (notaus == false)) {

		p[1] = p[0];
		p[0] = NULL;

		newPuk = true;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T1  EINLAUF_WERKSTUECK x  \n", p[1]->get_id(), p[1]->get_hoehenmessung1(), p[1]->get_typ());
		fflush(stdout);

	}

	/*_________T2_________*/
	if (p[1] != NULL && p[2] == NULL && (fehler == false) && (notaus == false)) {
		p[2] = p[1];
		p[1] = NULL;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T2		moving to H\n", p[2]->get_id(), p[2]->get_hoehenmessung1(), p[2]->get_typ());
		fflush(stdout);
	}

	/*_________T3_________*/
	if (p[2] != NULL && p[3] == NULL && (fehler == false) && (notaus == false)) {

		p[3] = p[2];
		p[2] = NULL;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T3		moving to H\n", p[3]->get_id(), p[3]->get_hoehenmessung1(), p[3]->get_typ());
		fflush(stdout);
	}

	/*_________T4_________*/
	if (p[3] != NULL && p[4] == NULL && (fehler == false) && (notaus == false)) {

		p[4] = p[3];
		p[3] = NULL;
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T4		moving to H\n", p[4]->get_id(), p[4]->get_hoehenmessung1(), p[4]->get_typ());
		fflush(stdout);
	}

	/*_________T5_________*/
	if (p[4] != NULL && p[5] == NULL && (petri_controller_1_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == true) && (fehler == false) && (notaus == false)) {

		p[5] = p[4];
		p[4] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		p[5]->set_hoehenmessung1(petri_controller_1_sen->getHeight());
		p[5]->set_typ(petri_controller_1_sen->getHeightPukType());

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T5	WERKSTUECK_IN_HOEHENMESSUNG\n", p[5]->get_id(), p[5]->get_hoehenmessung1(), p[5]->get_typ());
		fflush(stdout);

	}

	/*_________T6_________*/
	if (p[5] != NULL && p[6] == NULL && (petri_controller_1_inputs[WERKSTUECK_IN_HOEHENMESSUNG] == false) && (fehler == false) && (notaus == false)) {

		p[6] = p[5];
		p[5] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_SLOW_X)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T6	 WERKSTUECK_IN_HOEHENMESSUNG x\n", p[6]->get_id(), p[6]->get_hoehenmessung1(), p[6]->get_typ());
		fflush(stdout);

	}

	/*_________T7_________*/
	if (p[6] != NULL && p[7] == NULL && (fehler == false) && (notaus == false)) {

		p[7] = p[6];
		p[6] = NULL;
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T7		moving to W\n", p[7]->get_id(), p[7]->get_hoehenmessung1(), p[7]->get_typ());
		fflush(stdout);
	}

	/*_________T8________*/
	if (p[7] != NULL && p[8] == NULL && (fehler == false) && (notaus == false)) {

		p[8] = p[7];
		p[7] = NULL;
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T8		moving to W\n", p[8]->get_id(), p[8]->get_hoehenmessung1(), p[8]->get_typ());
		fflush(stdout);

	}

	/*_________T9_________*/
	if (p[8] != NULL && p[9] == NULL && (fehler == false) && (notaus == false)) {

		p[9] = p[8];
		p[8] = NULL;
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T9		moving to W\n", p[9]->get_id(), p[9]->get_hoehenmessung1(), p[9]->get_typ());
		fflush(stdout);

	}

	/*_________T10_________*/
	if (p[9] != NULL && p[10] == NULL && (petri_controller_1_inputs[WERKSTUECK_IN_WEICHE] == true) && (fehler == false) && (notaus == false)) {

		p[10] = p[9];
		p[9] = NULL;
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	 T10		WERKSTUECK_IN_WEICHE\n", p[10]->get_id(), p[10]->get_hoehenmessung1(), p[10]->get_typ());
		fflush(stdout);

	}

	/*_________T11	METAL_________*/
	if (p[10] != NULL && p[11] == NULL && (petri_controller_1_inputs[WERKSTUECK_METALL] == true) && (fehler == false) && (notaus == false)) {

		p[10]->set_typ(PUK_METALL);
		p[11] = p[10];
		p[10] = NULL;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T11		WERKSTUECK_METALL\n", p[11]->get_id(), p[11]->get_hoehenmessung1(), p[11]->get_typ());
		fflush(stdout);
	}

	/*_________T12	METAL x_________*/
	if (p[10] != NULL && p[11] == NULL && (petri_controller_1_inputs[WERKSTUECK_METALL] == false) && (fehler == false) && (notaus == false)) {

		p[11] = p[10];
		p[10] = NULL;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T12		WERKSTUECK_METALL x\n", p[11]->get_id(), p[11]->get_hoehenmessung1(), p[11]->get_typ());
		fflush(stdout);
	}

	/*_________T13_________*/
	if (p[11] != NULL && p[12] == NULL && (p[11]->get_typ() == PUK_GROSS || p[11]->get_typ() == PUK_LOCH || p[11]->get_typ() == PUK_METALL)
			&& (fehler == false) && (notaus == false)) {

		p[12] = p[11];
		p[11] = NULL;

		//gate1->open();
		petri_controller_1_outputs[WEICHE_AUF] = true;
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T13		FLACH x\n", p[12]->get_id(), p[12]->get_hoehenmessung1(), p[12]->get_typ());
		fflush(stdout);
	}

	/*_________T14_________*/
	if (p[12] != NULL && p[13] == NULL && (fehler == false) && (notaus == false)) {

		p[13] = p[12];
		p[12] = NULL;
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T14		TASTE_E_STOP x\n", p[13]->get_id(), p[13]->get_hoehenmessung1(), p[13]->get_typ());
		fflush(stdout);

	}

	/*_________T15_________*/
	if (p[13] != NULL && p[14] == NULL && (petri_controller_1_inputs[WERKSTUECK_IN_WEICHE] == false) && (fehler == false) && (notaus == false)) {

		p[14] = p[13];
		p[13] = NULL;

		timer_Gate->start();

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T15		WERKSTUECK_IN_WEICHE \n", p[14]->get_id(), p[14]->get_hoehenmessung1(), p[14]->get_typ());
		fflush(stdout);

	}

	/*_________T16_________*/
	if (p[14] != NULL && p[15] == NULL && gate_close_c1_timeout == true && (fehler == false) && (notaus == false)) {

		gate_close_c1_timeout = false;

		p[15] = p[14];
		p[14] = NULL;

		petri_controller_1_outputs[WEICHE_AUF] = false;
		timer_c1->deleteTimer(timer_Gate);
		timer_Gate = timer_c1->createTimer(petri_controller_1_sensorik_Chid, 0, C1_CLOSE_GATE_TIME, TIMER_GATE);

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T16	gate_close_c1_timeout == True\n", p[15]->get_id(), p[15]->get_hoehenmessung1(), p[15]->get_typ());
		fflush(stdout);

	}

	/*_________T17_________*/
	if (p[15] != NULL && p[16] == NULL && (fehler == false) && (notaus == false)) {

		p[16] = p[15];
		p[15] = NULL;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T17		moving to END\n", p[16]->get_id(), p[16]->get_hoehenmessung1(), p[16]->get_typ());
		fflush(stdout);

	}

	/*_________T18_________*/
	if (p[16] != NULL && p[17] == NULL && (fehler == false) && (notaus == false)) {

		p[17] = p[16];
		p[16] = NULL;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T18		moving to END\n", p[17]->get_id(), p[17]->get_hoehenmessung1(), p[17]->get_typ());
		fflush(stdout);

	}

	/*_________T19_________*/
	if (p[17] != NULL && p[18] == NULL && (fehler == false) && (notaus == false)) {

		p[18] = p[17];
		p[17] = NULL;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T19		moving to END\n", p[18]->get_id(), p[18]->get_hoehenmessung1(), p[18]->get_typ());
		fflush(stdout);

	}

	//ToDo Puk != Loch
	/*_________T20_________*/
	if (p[18] != NULL && p[19] == NULL && (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == true) && (fehler == false) && (p[18]->get_typ()
			!= PUK_LOCH) && (notaus == false)) {

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

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T20  AUSLAUF_WERKSTUECK & PUK != LOCH\n", p[19]->get_id(), p[19]->get_hoehenmessung1(),
				p[19]->get_typ());
		fflush(stdout);

	}

	/*_________T21_________*/
	if (p[19] != NULL && p[20] == NULL && (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == false) && (fehler == false) && (notaus == false)) {

		p[20] = p[19];
		p[19] = NULL;
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T21	AUSLAUF_WERKSTUECK x \n", p[20]->get_id(), p[20]->get_hoehenmessung1(), p[20]->get_typ());
		fflush(stdout);

	}

	/*_________T22_________*/
	if (p[20] != NULL && p[21] == NULL && (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == true) && (fehler == false)
			&& (disp_petri_controller_1->controller_2_free == true) && (notaus == false)) {

		p[21] = p[20];
		p[20] = NULL;

		//Puk Daten senden
		sc->send_puk_data_pkg(p[21]->get_id(), p[21]->get_typ(), p[21]->get_hoehenmessung1());

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP_X)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_GREEN)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T22	AUSLAUF_WERKSTUECK \n", p[21]->get_id(), p[21]->get_hoehenmessung1(), p[21]->get_typ());
		fflush(stdout);

	}

	/*_________T23_________*/
	if (p[21] != NULL && p[22] == NULL && (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == false) && (fehler == false) && (notaus == false)) {

		p[22] = p[21];
		p[21] = NULL;

		timer_move->start();
		sc->send_msg_pkg(P_CONVEYOR_START);

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T23	AUSLAUF_WERKSTUECK x \n", p[22]->get_id(), p[22]->get_hoehenmessung1(), p[22]->get_typ());
		fflush(stdout);

	}

	/*_________T24_________*/
	if (p[22] != NULL && (move_to_c1_timeout == true) && (fehler == false) && (notaus == false)) {

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T24	move_to_c2_timeout == true\n", p[22]->get_id(), p[22]->get_hoehenmessung1(), p[22]->get_typ());
		fflush(stdout);
		p[22] = NULL;
		gz++;

		printf("GZ : %d\n", gz);
		fflush(stdout);

		if (gz == 4) {
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
				exit(EXIT_FAILURE);
			}
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
			}
		}

		move_to_c1_timeout = false;
		timer_c1->deleteTimer(timer_move);
		timer_move = timer_c1->createTimer(petri_controller_1_sensorik_Chid, 0, C1_MOVE_TO_TIME, TIMER_MOVE);

		puts(" T24 PUK IST NICHT MEHR DA \n");
		fflush(stdout);
	}

	//ToDo PUK == Loch
	/*_________T25_________*/
	if (p[18] != NULL && p[23] == NULL && (petri_controller_1_inputs[AUSLAUF_WERKSTUECK] == true) && (fehler == false) && (p[18]->get_typ()
			== PUK_LOCH) && (notaus == false)) {

		p[23] = p[18];
		p[18] = NULL;

		end = true;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T25	AUSLAUF_WERKSTUECK & PUK LOCH\n", p[23]->get_id(), p[23]->get_hoehenmessung1(), p[23]->get_typ());
		fflush(stdout);
	}

	/*_________T26_________*/
	if (p[23] != NULL && p[21] == NULL && (disp_petri_controller_1->controller_2_free == true) && (fehler == false) && (notaus == false)) {

		p[21] = p[23];
		p[23] = NULL;

		//Puk Daten senden
		sc->send_puk_data_pkg(p[21]->get_id(), p[21]->get_typ(), p[21]->get_hoehenmessung1());

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T26	controller_2_free == true\n", p[21]->get_id(), p[21]->get_hoehenmessung1(), p[21]->get_typ());
		fflush(stdout);
	}

	/*_________T27_________*/
	if (p[23] != NULL && p[24] == NULL && (disp_petri_controller_1->controller_2_free == false) && (fehler == false) && (notaus == false)) {

		p[24] = p[23];
		p[23] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_YELLOW)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T27	controller_2_free == false\n", p[24]->get_id(), p[24]->get_hoehenmessung1(), p[24]->get_typ());
		fflush(stdout);
	}

	/*_________T28_________*/
	if (p[24] != NULL && p[21] == NULL && end == true && (disp_petri_controller_1->controller_2_free == true) && (fehler == false) && (notaus == false)) {
		end = false;
		p[21] = p[24];
		p[24] = NULL;

		//Puk Daten senden
		sc->send_puk_data_pkg(p[21]->get_id(), p[21]->get_typ(), p[21]->get_hoehenmessung1());

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP_X)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_GREEN)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T28	controller_2_free == true\n", p[21]->get_id(), p[21]->get_hoehenmessung1(), p[21]->get_typ());
		fflush(stdout);

	}

	//ToDo Rutsche

	/*_________T29_________*/
	if (p[11] != NULL && p[25] == NULL && p[11]->get_typ() == PUK_FLACH && (fehler == false) && (notaus == false)) {

		p[25] = p[11];
		p[11] = NULL;

		gate1->open();
		usleep(70000);
		gate1->close();

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T29		WERKSTUECK IST FLACH \n", p[25]->get_id(), p[25]->get_hoehenmessung1(), p[25]->get_typ());
		fflush(stdout);
	}

	/*_________T30_________*/
	if (p[25] != NULL && p[26] == NULL && (petri_controller_1_inputs[RUTSCHE_VOLL] == true) && (fehler == false) && (notaus == false)) {

		p[26] = p[25];
		p[25] = NULL;

		timer_C1_SlideFull->start();

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T30		moving to LS-RUTSCHE \n", p[26]->get_id(), p[26]->get_hoehenmessung1(), p[26]->get_typ());
		fflush(stdout);
	}

	/*_________T31_________*/
	if (p[26] != NULL && p[27] == NULL && (petri_controller_1_inputs[RUTSCHE_VOLL] == false) && (fehler == false) && (notaus == false)) {

		p[27] = p[26];
		p[26] = NULL;

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

		timer_c1->deleteTimer(timer_C1_SlideFull);
		timer_C1_SlideFull = timer_c1->createTimer(petri_controller_1_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T31		leaving LS-RUTSCHE \n", p[27]->get_id(), p[27]->get_hoehenmessung1(), p[27]->get_typ());
		fflush(stdout);
	}

	/*_________T32_________*/
	if (p[26] != NULL && p[28] == NULL && rutsche_voll_c1_timeout == true && (notaus == false)) {

		rutsche_voll_c1_timeout = false;

		p[28] = p[26];
		p[26] = NULL;

		fehler = true;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED_B)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		timer_c1->deleteTimer(timer_C1_SlideFull);
		timer_C1_SlideFull = timer_c1->createTimer(petri_controller_1_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T32 TIMEOUT SLIDEFULL \n", p[28]->get_id(), p[28]->get_hoehenmessung1(), p[28]->get_typ());
		fflush(stdout);
	}

	/*_________T33_________*/
	if (p[28] != NULL && p[29] == NULL && (petri_controller_1_inputs[TASTE_RESET] == true) && (notaus == false)) {

		p[29] = p[28];
		p[28] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}
		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T33	Q-TASTE GEDRUECKT \n", p[29]->get_id(), p[29]->get_hoehenmessung1(), p[29]->get_typ());
		fflush(stdout);
	}

	/*_________T34_________*/
	if (p[29] != NULL && p[30] == NULL && (petri_controller_1_inputs[RUTSCHE_VOLL] == false) && (notaus == false)) {

		p[30] = p[29];
		p[29] = NULL;

		printf("Puk(%d) Hoehe:%d  Typ:%d   ---->	T34 LSRX \n", p[30]->get_id(), p[30]->get_hoehenmessung1(), p[30]->get_typ());
		fflush(stdout);

	}

	/*_________T35_________*/
	if (p[30] != NULL && p[27] == NULL && (petri_controller_1_inputs[TASTE_START] == true) && (notaus == false)) {

		p[27] = p[30];
		p[30] = NULL;

		fehler = false;
		printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<%d \n", gz);
		fflush(stdout);

		if (gz == 3) {
			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
				exit(EXIT_FAILURE);
			}

			if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_END)) {
				perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
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

		printf("Puk(%d) Hoehe:%d  Typ:%d   ----> T35	S-TASTE GEDRUECKT \n", p[27]->get_id(), p[27]->get_hoehenmessung1(), p[27]->get_typ());
		fflush(stdout);

	}

	/*_________T36_________*/
	if (p[27] != NULL && (notaus == false)) {

		gz++;

		timer_c1->deleteTimer(timer_C1_SlideFull);
		timer_C1_SlideFull = timer_c1->createTimer(petri_controller_1_sensorik_Chid, SLIDE_FULL_TIME, 0, TIMER_FULL);

		printf("Puk(%d) Hoehe:%d  Typ:%d   ----> T36 to GZ \n", p[27]->get_id(), p[27]->get_hoehenmessung1(), p[27]->get_typ());
		fflush(stdout);
		p[27] = NULL;
	}

	//ToDo PUK Schwindet LSH

	/*_________T37_________*/
	if (p[4] != NULL && p[31]== NULL && (removed_C1_timeout_LSH == true)&&(notaus ==false)) {

		removed_C1_timeout_LSH = false;

		p[31] = p[4];
		p[4] = NULL;

		timer_c1->deleteTimer(timer_C1_removed_LSH);
		timer_C1_removed_LSH = timer_c1->createTimer(petri_controller_1_sensorik_Chid, 4, 0, TIMER_REMOVED_C1_LSH);

		printf("Puk(%d) Hoehe:%d  Typ:%d   ----> T37 PUK Verwindet  LSH \n", p[31]->get_id(), p[31]->get_hoehenmessung1(), p[31]->get_typ());
		fflush(stdout);
	}



	//ToDo ERROR ZUSTAND

	/*_________T43_________*/
	if (p[31] != NULL && p[32]== NULL && (notaus ==false)) {

		p[32] = p[31];
		p[31] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_STOP)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED_B)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		printf("Puk(%d) Hoehe:%d  Typ:%d   ----> T43 ERROR NICHT QUITIERT \n", p[32]->get_id(), p[32]->get_hoehenmessung1(), p[32]->get_typ());
		fflush(stdout);
	}

	/*_________T44_________*/
	if (p[32] != NULL && p[33]== NULL && (petri_controller_1_inputs[TASTE_RESET] == false) && (notaus ==false)) {

		p[33] = p[32];
		p[32] = NULL;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_RED)) {
					perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
					exit(EXIT_FAILURE);
		}

		printf("Puk(%d) Hoehe:%d  Typ:%d   ----> T43 ERROR QUITIERT \n", p[33]->get_id(), p[33]->get_hoehenmessung1(), p[33]->get_typ());
		fflush(stdout);
	}

	/*_________T45_________*/
	if (p[33] != NULL && (petri_controller_1_inputs[TASTE_START] == true) && (notaus ==false)) {

		gz++;

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_CONVEYOR, P_CONVEYOR_END)) {
			perror("Petri_Controller_1:: MsgSendPulse an coveyour\n");
			exit(EXIT_FAILURE);
		}

		if (-1 == MsgSendPulse(petri_controller_1_dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT, PA_TRAFFICLIGHT, TRAFFICLIGHT_END)) {
			perror("Petri_Controller_1:: MsgSendPulse an trafficLight\n");
			exit(EXIT_FAILURE);
		}

		printf("Puk(%d) Hoehe:%d  Typ:%d   ----> T43 ERROR NICHT QUITIERT \n", p[33]->get_id(), p[33]->get_hoehenmessung1(), p[33]->get_typ());
		fflush(stdout);
		p[33] = NULL;
	}

}

void Petri_Controller_1::calculate_outputs() {
}

void Petri_Controller_1::NotifyReactor() {

	if (petri_controller_1_outputs[WEICHE_AUF] == true) {
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

	//	printf("nachher: petri_controller_1_inputs[TASTE_E_STOP] = %d\n", petri_controller_1_inputs[RUTSCHE_VOLL]); fflush(stdout);
	//	printf("nachher: tmpArr_1[TASTE_E_STOP] = %d\n", tmpArr_1[RUTSCHE_VOLL]); fflush(stdout);
}

void Petri_Controller_1::shutdown() {

}

