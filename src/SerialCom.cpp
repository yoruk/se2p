#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include "hal/Serial.h"
#include "Mutex.h"
#include "Global.h"
#include "SerialCom.h"

/// This class's methods can transfer packages
/// with puk information or status messages over the serial line.
/// It also holds a background process
/// which receives packages and informs the dispatcher about them.

typedef struct __attribute__((__packed__)) {
		uint8_t is_msg;
		uint8_t msg_content;
		uint8_t puk_id;
		uint8_t puk_type;
		uint16_t puk_height;
} package;

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static SerialCom* serialCom; /// the SerialCom object itself
static Serial* serial; /// the Serial object for basic i/o-functions
static pthread_t rcv_pkgs;

/// this is a pthread who receives packages,
///	checks their content
/// and sends suitable pulse messages to the dispatcher
void* receive_pkgs(void* arg) {
	int nodeId = 0; // 0 = local
	pid_t pid = 0; // 0 = same process
	int connectionId = 0;
	int res = 0;;
	package p;
	int pulse_val = 0;

	connectionId = ConnectAttach(nodeId, pid, *(int*)arg, 0, 0);
	if(connectionId == -1) {
		perror("SerialCom: ERROR, couldn't attach to channel");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	while(TRUE) {
		res = serial->read_serial((unsigned char*)&p, sizeof(p));

		// was the transfer successfully ?
		if(res == sizeof(p)) {

			// what type of package ?
			if(p.is_msg) {

				// package holds a message
				res = MsgSendPulse(connectionId, SIGEV_PULSE_PRIO_INHERIT, PULSE_MSG_FROM_CONVEYOR, p.msg_content);
				if(res == -1) {
					perror("SerialCom: ERROR, couldn't send pulse message with puk information");fflush(stdout);
					exit(EXIT_FAILURE);
				}

				//DEBUG
//				printf("pulse message with code = %d, value = %d has been sent\n",
//						PULSE_MSG_FROM_CONVEYOR, p.msg_content);fflush(stdout);

			} else {

				// package holds puk information
				pulse_val = p.puk_id;
				pulse_val <<= 8;
				pulse_val |= p.puk_type;
				pulse_val <<= 16;
				pulse_val |= p.puk_height;

				res = MsgSendPulse(connectionId, SIGEV_PULSE_PRIO_INHERIT, PULSE_PUK_INFORMATION, pulse_val);
				if(res == -1) {
					perror("SerialCom: ERROR, couldn't send pulse message with puk information");fflush(stdout);
					exit(EXIT_FAILURE);
				}

				//DEBUG
//				printf("pulse message with code = %d, value = %d has been sent\n",
//						PULSE_PUK_INFORMATION, pulse_val);fflush(stdout);
			}


		} else {
			perror("SerialCom: ERROR, package receive failed");fflush(stdout);
		}

	}

	pthread_exit(NULL);
}

/// Serialcom-constructor
SerialCom::SerialCom() {
	serial = Serial::getInstance();
}

/// SerialCom-deconstructor
SerialCom::~SerialCom() {
	if(pthread_cancel(rcv_pkgs) != 0) {
		perror("SerialCom: ERROR, couldn't cancel receive_pkgs() properly");fflush(stdout);
	}

	pthread_join(rcv_pkgs, NULL);

	delete serial;
	delete serialCom;
	serialCom = NULL;
}

/// returns the only running instance of SerialCom
SerialCom* SerialCom::getInstance() {
	mutex->lock();

	if (!serialCom) {
		serialCom = new SerialCom();
	}

	mutex->unlock();

	return serialCom;
}

/// starts the background process for receiving packages
///
/// \param Chid channel id where pulse messages should go to
///
/// \return 0=successful, 1=failed
int SerialCom::start_pkg_receiver(int Chid) {
	int res = 0;

	//printf("DEBUG:SerialCom: starting package receiver\n");fflush(stdout);

	res = pthread_create(&rcv_pkgs, NULL, &receive_pkgs, (void*)Chid);
	if(res != 0) {
		perror("SerialCom: ERROR, couldn't start receive_pkgs() properly");fflush(stdout);

		return res;
	}

	pthread_detach(rcv_pkgs);

	return res;
}

/// send a package with puk data
///
/// \param puk_id id of the puk the data is about
/// \param puk_type type of the puk
/// \param puk_height height of the puk
///
/// \return 0=successful, 1=failed
int SerialCom::send_puk_data_pkg(int puk_id, int puk_type, int puk_height) {
	int res = 0;
	package p;

	//printf("DEBUG:SerialCom: sending puk package\n");fflush(stdout);

	// wrapping package
	p.is_msg = FALSE;
	p.msg_content = 0;
	p.puk_id = (uint16_t)puk_id;
	p.puk_type = (uint8_t)puk_type;
	p.puk_height = (uint16_t)puk_height;

	// sending package
	res = serial->read_serial((unsigned char*)&p, sizeof(p));

	return (res == sizeof(p)) ? 0 : 1;
}

/// sends a package with a status message
///
/// \param msg_content the content-code of the status message
///
/// \return 0=successful, 1=failed
int SerialCom::send_msg_pkg(int msg_content) {
	int res = 0;
	package p;

	//printf("DEBUG:SerialCom: sending message package\n");fflush(stdout);

	// wrapping package
	p.is_msg = TRUE;
	p.msg_content = msg_content;
	p.puk_id = 0;
	p.puk_type = 0;
	p.puk_height = 0;

	// sending package
	res = serial->write_serial((unsigned char*)&p, sizeof(p));

	return (res == sizeof(p)) ? 0 : 1;
}
