#include "Sensorik.h"

static int isr_coid;
static Mutex* mutex = new Mutex(); /// the mutex for controlling the access

const struct sigevent* ISR(void* arg, int id) {
	struct sigevent *event = (struct sigevent *) arg;
	uint16_t val;
	uint8_t state;
	state = in8(D_IOBASE + OFFS_INT_STATUS) & (BIT_1 | BIT_3); // lesen
	out8(D_IOBASE + OFFS_INT_STATUS, 0); // zuruecksetzen

	switch (state) {
	case (BIT_1 | BIT_3):
		val = (in8(DIO_B) << 8) | (in8(DIO_C));
		SIGEV_PULSE_INIT(event, isr_coid, SIGEV_PULSE_PRIO_INHERIT,
				PB_STATUS | PC_STATUS, val);
		break;
	case BIT_1:
		val = in8(DIO_B);
		SIGEV_PULSE_INIT(event, isr_coid, SIGEV_PULSE_PRIO_INHERIT,
				PB_STATUS, val);
		break;
	case BIT_3:
		val = in8(DIO_C);
		SIGEV_PULSE_INIT(event, isr_coid, SIGEV_PULSE_PRIO_INHERIT,
				PC_STATUS, val);
		break;
	default:
		event = NULL;
	}
	return event;
}

Sensorik* Sensorik::instance = NULL;

Sensorik::Sensorik() {
	initInterrupts();

	// create channel for dispatcher
	signalChid = ChannelCreate(0);
	if (signalChid == -1) {
		perror("Dispatcher: ChannelCreate signalChid failed");
		exit(EXIT_FAILURE);
	}

	// attach to signal channel(stellt die verbindung zu dem channel des Prozesses PID)
	signalCoid = ConnectAttach(0, 0, signalChid, _NTO_SIDE_CHANNEL, 0);
	if (signalCoid == -1) {
		perror("SensorCtrl: ConnectAttach signalCoid failed");
		exit(EXIT_FAILURE);
	}
}

Sensorik::~Sensorik() {
	if (instance != NULL) {
		delete instance;
		instance = NULL;
	}
}

Sensorik* Sensorik::getInstance() {
	mutex->lock();
	if (!init_HW_Done()) {
		init_HW();
	}

	if (instance == NULL) {
		instance = new Sensorik();
	}
	mutex->unlock();

	return instance;
}

void Sensorik::initInterrupts() {
	// create channel to receive pulse messages from the ISR
	isrChid = ChannelCreate(0);
	if (isrChid == -1) {
		perror("SensorikIntro: ChannelCreate isrChid failed");
		exit(EXIT_FAILURE);
	}
	isr_coid = ConnectAttach(0, 0, isrChid, _NTO_SIDE_CHANNEL, 0);
	if (isr_coid == -1) {
		perror("SensorikIntro: ConnectAttach isr_coid failed");
		exit(EXIT_FAILURE);
	}

	// attach ISR
	SIGEV_PULSE_INIT(&event, isr_coid, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
	//attach conection between handler to an interrupt source
	interruptId = InterruptAttach(11, ISR, &event, sizeof(event), 0);
	if (interruptId == -1) {
		perror("SensorikIntro: InterruptAttach failed");
		exit(EXIT_FAILURE);
	}

	// configure interrupts

	// reset irq status register
	out8(D_IOBASE + OFFS_INT_STATUS, 0);
	// disable interrupts for all ports (Bit 0-5)
	uint8_t int_ctrl = in8(D_IOBASE + OFFS_INT_CTRL);
	out8(D_IOBASE + OFFS_INT_CTRL, int_ctrl | 0b00111111);
	// enable interrupt for portB und portC (Bit 1 und 2) (S. 18)
	int_ctrl = in8(D_IOBASE + OFFS_INT_CTRL);
	out8(D_IOBASE + OFFS_INT_CTRL, int_ctrl & ~(PB_CTRL | PC_CTRL));

	// read out port B and C valies
	portBstatus = in8(DIO_B);
	portCstatus = in8(DIO_B);
}

void Sensorik::stop() {
	HAWThread::stop(); // super.stop();

	if (-1 == ConnectDetach(isr_coid)) {
		perror("SensorCtrl: ConnectDetach isr_coid failed");
	}
	if (-1 == ChannelDestroy(isrChid)) {
		perror("SensorCtrl: ChannelDestroy isr_chid failed");
	}
	// in Simulation: bleibt hier haengen
	//    if (-1 == InterruptDetach(interruptId)) {
	//        perror("SensorCtrl: InterruptDetach failed");
	//    }
}

void Sensorik::shutdown() {
}

void Sensorik::execute(void *arg) {

	struct _pulse pulse;
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(isrChid, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("SensorCtrl: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}
		MsgSendPulse(signalCoid, SIGEV_PULSE_PRIO_INHERIT, pulse.code, pulse.value.sival_int);
	}

}

int Sensorik::getSignalChid(){
        return signalChid;
}
