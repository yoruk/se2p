#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/neutrino.h>
#include "hal/Serial.h"
#include "Mutex.h"
#include "Global.h"
#include "SerialCom.h"

typedef struct __attribute__((__packed__)) {
		uint8_t is_msg;
		uint8_t msg_content;
		uint16_t puk_id;
		uint8_t puk_type;
		uint16_t puk_height;
} package;

/// This class's methods can transfer packages
/// with puk information or status messages over the serial line.
/// It also holds a background process
/// which receives packages and informs the dispatcher about them.

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Serial* serial; /// the Serial object for basic i/o-functions
static SerialCom* serialCom; /// the SerialCom object itself
static int channel_id; /// channel to the dispatcher

/// Serialcom-constructor
SerialCom::SerialCom() {
	serial = Serial::getInstance();
	serial->open_serial();
}

/// SerialCom-deconstructor
SerialCom::~SerialCom() {
	serial->close_serial();

	delete serial;
	serial = NULL;

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

	printf("DEBUG:SerialCom: sending puk package\n");fflush(stdout);

	// wrapping package
	p.is_msg = FALSE;
	p.msg_content = 0;
	p.puk_id = (uint16_t)puk_id;
	p.puk_type = (uint8_t)puk_type;
	p.puk_height = (uint16_t)puk_height;

	// sending package
	res = serial->write_serial((unsigned char*)&p, sizeof(p));

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
	p.msg_content = (uint8_t)msg_content;
	p.puk_id = 0;
	p.puk_type = 0;
	p.puk_height = 0;

	// sending package
	res = serial->write_serial((unsigned char*)&p, sizeof(p));

	return (res == sizeof(p)) ? 0 : 1;
}

/// sets the channel id, needs to be set BEFORE the background thread is started
///
/// \param chid channel id for communication with dispatcher
///
/// \return 0=successful, 1=failed
int SerialCom::set_Channel(int chid) {
	if(!serialCom) {
		return 1;
	} else {
		channel_id = chid;

		return 0;
	}
}

/// this is thread receives packages,
///	checks their content
/// and sends suitable pulse messages to the dispatcher
void SerialCom::execute(void *arg) {
	int nodeId = 0; // 0 = local
	pid_t pid = 0; // 0 = same process
	int connectionId = 0;
	int res = 0;;
	package p;
	int pulse_val = 0;

	connectionId = ConnectAttach(nodeId, pid, channel_id, 0, 0);
	if(connectionId == -1) {
		perror("SerialCom: ERROR, couldn't attach to channel");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	while(!isStopped() && (channel_id != 0)) {
		// read package
		res = serial->read_serial((unsigned char*)&p, sizeof(p));

		// was the transfer successfully ?
		if(res == sizeof(p)) {

			// what type of package ?
			if(p.is_msg) {

				// package holds a message
				res = MsgSendPulse(connectionId, SIGEV_PULSE_PRIO_INHERIT, PULSE_OTHER_MACHINE_STATUS, p.msg_content);
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
				pulse_val <<= PUK_TYPE_LENGTH;
				pulse_val |= p.puk_type;
				pulse_val <<= PUK_HEIGHT_LENGTH;
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
}

void SerialCom::shutdown() {

}

