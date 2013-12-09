#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include "../Global.h"
#include "../SerialCom.h"

#define CYCLES 5
#define WAIT 2

void test_SerialCom_start() {
	SerialCom* serCom;
	int channelId;
	int i = 0;

	serCom = SerialCom::getInstance();

	printf("SerialCom test!\n");fflush(stdout);

	channelId = ChannelCreate(0);
	if(channelId == -1) {
		perror("Test_SerialCom: ERROR, couldn't create channel");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	serCom->start_pkg_receiver(channelId);

	sleep(WAIT);

	// send puk data CYCLES times
	printf("Test_SerialCom: sending puk data %d times\n", CYCLES);fflush(stdout);
	while(i < CYCLES) {
		if(serCom->send_puk_data_pkg(i, i, i) != 0) {
			perror("Test_SerialCom: ERROR, couldn't send puk data package");fflush(stdout);
		}

		i++;
	}

	// send status messages CYCLES times
	printf("Test_SerialCom: sending status messages %d times\n", CYCLES);fflush(stdout);
	while(i < CYCLES) {
		if(serCom->send_msg_pkg(i) != 0) {
			perror("Test_SerialCom: ERROR, couldn't send status message package");fflush(stdout);
		}

		i++;
	}

	delete serCom;
	serCom = NULL;
}
