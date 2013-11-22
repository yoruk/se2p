#include "Dispatcher.h"

static Mutex* mutex = new Mutex();
bool pB[8] = { false, false, false, false, false, false, false, false };
bool pC[8] = { false, false, false, false };

hal::Dispatcher* hal::Dispatcher::instance = NULL;

hal::Dispatcher::Dispatcher() {
	hal::Sensorik* sen = hal::Sensorik::getInstance();
	signalChid = sen->getSignalChid();
}

hal::Dispatcher::~Dispatcher() {
	delete instance;
	instance = NULL;
}

hal::Dispatcher* hal::Dispatcher::getInstance() {
	mutex->lock();
	if (!init_HW_Done()) {
		init_HW();
	}

	if (!instance) {
		instance = new Dispatcher();
	}
	mutex->unlock();

	return instance;
}

void hal::Dispatcher::execute(void* arg) {
	struct _pulse pulse;
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(signalChid, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("SensorCtrl: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}
		setSensor(pulse.code, pulse.value.sival_int);
	}
}

void hal::Dispatcher::shutdown() {
}

void hal::Dispatcher::setSensor(int code, int value) {

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
