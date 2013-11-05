/*
 * SensorikIntro.cpp
 */

#include "Sensorik.h"
#include "HWaccess.h"
#include "Global.h"
#include "hw.h"

static int isr_coid;
bool pB[8] = { false, false, false, false, false, false, false, false };
bool pC[8] = { false, false, false, false };
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

hal::Sensorik* hal::Sensorik::instance = NULL;

hal::Sensorik::Sensorik() {
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

hal::Sensorik::~Sensorik() {
	if (instance != NULL) {
		delete instance;
		instance = NULL;
	}
}

hal::Sensorik* hal::Sensorik::getInstance() {
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

void hal::Sensorik::initInterrupts() {
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

void hal::Sensorik::stop() {
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

void hal::Sensorik::shutdown() {
}

void hal::Sensorik::execute(void *arg) {
	//code: 8-bit beschreibung der pulse msg
	//value: 32-bit daten
	cout << "| code  |  " << "value |" << endl;
	cout << "|----------------|" << endl;

	struct _pulse pulse;
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(isrChid, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("SensorCtrl: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}
		print(pulse.code, pulse.value.sival_int);
	}

}
void hal::Sensorik::print(int code, int value) {

	switch (code) {
	case 2:
		if (((value & BIT_0) == 0) && !pB[0]) {
			cout << "LS-Eingang unterbrochen" << endl;
			pB[0] = true;
		} else if ((value & BIT_0) && pB[0]) {
			cout << "LS-Eingang frei" << endl;
			pB[0] = false;
		}

		if (((value & BIT_1) == 0) && !pB[1]) {
			cout << "LS-Hoehenmessung unterbrochen" << endl;
			pB[1] = true;
		} else if ((value & BIT_1) && pB[1]) {
			cout << "LS-Hoehenmessung frei" << endl;
			pB[1] = false;
		}

		if ((value & BIT_2) && !pB[2]) {
			cout << "Werkstueck nicht flach" << endl;
			pB[2] = true;
		} else if (((value & BIT_2) == 0) && pB[2]) {
			cout << "Werkstueck flach" << endl;
			pB[2] = false;
		}

		if (((value & BIT_3) == 0) && !pB[3]) {
			cout << "LS-Weiche unterbrochen" << endl;
			pB[3] = true;
		} else if ((value & BIT_3) && pB[3]) {
			cout << "LS-Weiche frei" << endl;
			pB[3] = false;
		}

		if ((value & BIT_4) && !pB[4]) {
			cout << "Werkstueck Metall" << endl;
			pB[4] = true;
		} else if (((value & BIT_4) == 0) && pB[4]) {
			cout << "Werkstueck kein Metall" << endl;
			pB[4] = false;
		}

		if ((value & BIT_5) && !pB[5]) {
			cout << "Weiche offen" << endl;
			pB[5] = true;
		} else if (((value & BIT_5) == 0) && pB[5]) {
			cout << "Weiche geschlossen" << endl;
			pB[5] = false;
		}

		if (((value & BIT_6) == 0) && !pB[6]) {
			cout << "Rutsche ist voll" << endl;
			pB[6] = true;
		} else if ((value & BIT_6) && pB[6]) {
			cout << "Rutsche nicht voll" << endl;
			pB[6] = false;
		}

		if (((value & BIT_7) == 0) && !pB[7]) {
			cout << "LS-Auslauf unterbrochen" << endl;
			pB[7] = true;
		} else if ((value & BIT_7) && pB[7]) {
			cout << "LS-Auslauf frei" << endl;
			pB[7] = false;
		}
		break;
	case 8:
		if ((value & BIT_4) && !pC[0]) {
			cout << "Starttaste gedrueckt" << endl;
			pC[0] = true;
		} else if (((value & BIT_4) == 0) && pC[0]) {
			cout << "Starttaste nicht gedrueckt" << endl;
			pC[0] = false;
		}

		if (((value & BIT_5) == 0) && !pC[1]) {
			cout << "Stoptaste gedrueckt" << endl;
			pC[1] = true;
		} else if ((value & BIT_5) && pC[1]) {
			cout << "Stoptaste nicht gedrueckt" << endl;
			pC[1] = false;
		}

		if ((value & BIT_6) && !pC[2]) {
			cout << "Resettaste gedrueckt" << endl;
			pC[2] = true;
		} else if (((value & BIT_6) == 0) && pC[2]) {
			cout << "Resettaste nicht gedrueckt" << endl;
			pC[2] = false;
		}

		if (((value & BIT_7) == 0) && !pC[3]) {
			cout << "E-stop gedrueckt" << endl;
			pC[3] = true;
		} else if ((value & BIT_7) && pC[3]) {
			cout << "E-stop nicht gedrueckt" << endl;
			pC[3] = false;
		}
		break;
	}
}
