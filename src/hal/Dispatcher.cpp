#include "Dispatcher.h"

static Mutex* mutex = new Mutex();

Dispatcher* Dispatcher::instance = NULL;

Dispatcher::Dispatcher() {
	Sensorik* sen = Sensorik::getInstance();
	pet = Petrib::getInstance();
	signalChid = sen->getSignalChid();
}

Dispatcher::~Dispatcher() {
	delete instance;
	instance = NULL;
}

Dispatcher* Dispatcher::getInstance() {
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

void Dispatcher::execute(void* arg) {
	struct _pulse pulse;
	pet->init_places();
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(signalChid, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("SensorCtrl: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}

		pet->read_inputs(pulse.code, pulse.value.sival_int);
		pet->process_transitios();
		pet->calculate_outputs();
		//pet->write_outputs();
		pet->NotifyReactor();
	}
}

void Dispatcher::shutdown() {
}


