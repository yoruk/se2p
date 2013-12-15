#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include "../Global.h"
#include "../SerialCom.h"
#include "../Puk.h"

#define CYCLES 5
#define RUNTIME 60
#define WAIT_SHORT 1
#define WAIT_LONG 5

SerialCom* sc;
static int channelId = 0;
static int run = TRUE;
static pthread_t disp;
static pthread_t conv;

static void* dispatcher(void* arg) {
	struct _pulse pulse;
	Puk* p;
	int res = 0;

	printf("DEBUG:Test_SerialCom: dispatcher: is running\n");fflush(stdout);

	while(run) {
		res = MsgReceivePulse(channelId, &pulse, sizeof(pulse), NULL);
		if(res  == -1) {
			perror("DEBUG:Test_SerialCom: dispatcher: ERROR, couldn't receive pulse");fflush(stdout);
			exit(EXIT_FAILURE);
		}

		if(pulse.code == PULSE_MSG_FROM_CONVEYOR) {

			// status message from conveyor
			printf("DEBUG:Test_SerialCom: dispatcher: message from conveyor arrived, ");fflush(stdout);
			printf("code is %d\n", pulse.value.sival_int);fflush(stdout);

		} else if(pulse.code == PULSE_PUK_INFORMATION) {

			// message with puk information
			printf("DEBUG:Test_SerialCom: dispatcher: message with puk information arrived, \n");fflush(stdout);

			p = Puk::intToPuk(pulse.value.sival_int);

			printf("puk_id = %d\n", p->get_id());fflush(stdout);
			printf("puk_type = %d\n", p->get_typ());fflush(stdout);
			printf("puk_height = %d\n", p->get_hoehenmessung1());fflush(stdout);

		} else {

			// unknown message
			printf("DEBUG:Test_SerialCom: dispatcher: unknown message arrvied\n");fflush(stdout);
		}
	}

	printf("DEBUG:Test_SerialCom: dispatcher: ended\n");fflush(stdout);

	pthread_exit(NULL);
}

static void* conveyor(void* arg) {
	int i = 0;

	printf("DEBUG:Test_SerialCom: conveyor: is running\n");fflush(stdout);

	while(run) {
		// send puk data CYCLES times
		printf("Test_SerialCom: conveyor: sending puk data %d times\n", CYCLES);fflush(stdout);
		while(i < CYCLES) {
			if(sc->send_puk_data_pkg(i, i, i) != 0) {
				perror("Test_SerialCom: conveyor: ERROR, couldn't send puk data package");fflush(stdout);
			}

			printf("Test_SerialCom: conveyor: sending puk data #%d, done\n", i);fflush(stdout);

			i++;

			sleep(WAIT_SHORT);
		}

		i = 0;

		// send status messages CYCLES times
		printf("Test_SerialCom: conveyor: sending status messages %d times\n", CYCLES);fflush(stdout);
		while(i < CYCLES) {
			if(sc->send_msg_pkg(i) != 0) {
				perror("Test_SerialCom: conveyor: ERROR, couldn't send status message package");fflush(stdout);
			}

			printf("Test_SerialCom: conveyor: sending status message #%d, done\n", i);fflush(stdout);

			i++;

			sleep(WAIT_SHORT);
		}

		i = 0;
	}

	printf("DEBUG:Test_SerialCom: conveyor: ended\n");fflush(stdout);

	pthread_exit(NULL);
}

void test_SerialCom_start() {
	printf("SerialCom test!\n");fflush(stdout);

	channelId = ChannelCreate(0);
	if(channelId == -1) {
		perror("Test_SerialCom: ERROR, couldn't create channel");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	pthread_create(&disp, NULL, &dispatcher, NULL);

	sc = SerialCom::getInstance();
	sc->set_Channel(channelId);
	sc->start(NULL);

	sleep(WAIT_LONG);
	pthread_create(&conv, NULL, &conveyor, NULL);

//	sleep(RUNTIME);
//	run = FALSE;
//	pthread_cancel(disp);

	pthread_join(disp, NULL);
	pthread_join(conv, NULL);

	sc->stop();
	sc->join();

	delete sc;
	sc = NULL;
}
