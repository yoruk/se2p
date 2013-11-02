#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <sys/neutrino.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <errno.h>

#define WAIT 5
#define WAIT_SHORT 1

#define MY_PULSE_CODE _PULSE_CODE_MINAVAIL
#define MSG_GIVE_PULSE 12

struct my_msg {
	short type;
	struct sigevent event;
};

static pthread_t t1, t2;
static int server_channelId = -1;

static void* server(void* arg) {
	int receiveId = 0;
	int res = 0;
	struct my_msg msg;

	/********************** Setup **********************/

	// create own channel
	server_channelId = ChannelCreate(0);
	if(server_channelId == -1) {
		printf("msgDeliverEvent: server: error creating channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	/********************** Run **********************/

	// wait for message from client
	receiveId = MsgReceive(server_channelId, &msg, sizeof(msg), NULL);
	if(receiveId == -1) {
		printf("msgDeliverEvent: server: error receiving message!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	// send reply to client
	res = MsgReply(receiveId, EOK, NULL, 0);
	if(res == -1) {
		printf("msgDeliverEvent: server: error replying message!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	if(msg.type == MSG_GIVE_PULSE) {
		sleep(2);

		res = MsgDeliverEvent(receiveId, &msg.event);
		if(res == -1) {
			printf("msgDeliverEvent: server: error sending event!\n");fflush(stdout);
			exit(EXIT_FAILURE);
		}

		printf("server:delivered event\n");
	} else {
		printf("server: unexpected message \n");
	}

	sleep(WAIT);

	/********************** Clean **********************/

	res = ChannelDestroy(server_channelId);
	if(res == -1) {
		printf("msgDeliverEvent: server: error destroying channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);
}

static void* client(void* arg) {
	int nodeId = 0; // 0 = local
	pid_t pid = 0; // 0 = same process
	int channelId = -1;
	int connectionId = 0;
	int server_connectionId = 0;
	int receiveId = 0;
	int res = 0;
	struct my_msg msg;
	struct _pulse pulse;

	/********************** Setup **********************/

	// create own channel to be able to receive pulse
	channelId = ChannelCreate(0);
	if(channelId == -1) {
		printf("msgDeliverEvent: client: error creating channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	// connect to that channel
	connectionId = ConnectAttach(nodeId, pid, channelId, _NTO_SIDE_CHANNEL, 0);
	if(connectionId == -1) {
		printf("msgDeliverEvent: client: error attaching to own channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	// prepare even structure for a pulse
	SIGEV_PULSE_INIT(&msg.event, connectionId, SIGEV_PULSE_PRIO_INHERIT, MY_PULSE_CODE, 0);
	msg.type = MSG_GIVE_PULSE;

	// connect to server channel
	server_connectionId = ConnectAttach(nodeId, pid, server_channelId, 0, 0);
	if(connectionId == -1) {
		printf("msgDeliverEvent: client: error attaching to server channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	/********************** Run **********************/

	// send a synchronous message to the server, message has event
	res = MsgSend(server_connectionId, &msg, sizeof(msg), NULL, 0);
	if(res == -1) {
		printf("msgDeliverEvent: client: error during sending message!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	// wait for the pulse
	receiveId = MsgReceivePulse(channelId, &pulse, sizeof(pulse), NULL);
	if(receiveId == -1) {
		printf("msgDeliverEvent: client: error receiving pulse!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	printf("got pulse with code %d, waiting for %d\n", pulse.code, MY_PULSE_CODE);

	/********************** Clean **********************/

	res = ConnectDetach(server_connectionId);
	if(res == -1) {
		printf("msgDeliverEventy: client: error detaching from channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	res = ConnectDetach(connectionId);
	if(res == -1) {
		printf("msgDeliverEventy: client: error detaching from channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	res = ChannelDestroy(channelId);
	if(res == -1) {
		printf("msgDeliverEvent: server: error destroying channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);
}

void msgDeliverEvent_start() {
	printf("running msgDeliverEvent_start()\n");fflush(stdout);

	pthread_create(&t1, NULL, &server, NULL);
	pthread_create(&t2, NULL, &client, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
