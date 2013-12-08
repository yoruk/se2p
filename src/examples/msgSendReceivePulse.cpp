#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/neutrino.h>
#include <errno.h>

#define RUNTIME 20
#define WAIT 5
#define WAIT_SHORT 1

static pthread_t t1, t2;
static bool run = true;
static int channelId = -1;

static void* server(void* arg) {
	int number = 0;
	struct _pulse pulse;
	int res = 0;

	channelId = ChannelCreate(0);
	if(channelId == -1) {
		printf("msgSendReceivePulse: server: error creating channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	sleep(5);

	while(run) {
		res = MsgReceivePulse(channelId, &pulse, sizeof(pulse), NULL);
		if(res  == -1) {
			printf("msgSendReceivePulse: server: error receiving pulse!\n");fflush(stdout);
			exit(EXIT_FAILURE);
		}

		number++;
		printf("Server: Pulse message #%d received\n", number);fflush(stdout);
	}

	res = ChannelDestroy(channelId);
	if(res == -1) {
		printf("msgSendReceivePulse: server: error destroying channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);
}

static void* client(void* arg) {
	int nodeId = 0; // 0 = local
	pid_t pid = 0; // 0 = same process
	int connectionId = 0;
	int number = 0;
	int res = 0;

	connectionId = ConnectAttach(nodeId, pid, channelId, 0, 0);
	if(connectionId == -1) {
		printf("msgSendReceivePulse: client: error attaching to channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	while(run) {
		sleep(WAIT_SHORT);

		number++;
		printf("Client: sending pulse message #%d\n", number);fflush(stdout);

		res = MsgSendPulse(connectionId, SIGEV_PULSE_PRIO_INHERIT, 1, 0);
		if(res == -1) {
			printf("msgSendReceivePulse: client: error sending pulse!\n");fflush(stdout);
			exit(EXIT_FAILURE);
		}
	}

	ConnectDetach(connectionId);
	if(res == -1) {
		printf("msgSendReceiveReply: client: error detaching from channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);
}

void msgSendReceivePulse_start() {
	printf("running msgSendReceivePulse_start()\n");fflush(stdout);

	pthread_create(&t1, NULL, &server, NULL);
	pthread_create(&t2, NULL, &client, NULL);

	sleep(RUNTIME);

	run = false;

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
