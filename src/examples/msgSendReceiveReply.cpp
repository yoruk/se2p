#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
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
	int receiveId = 0;
	char msgBuffer[256];
	int res = 0;

	channelId = ChannelCreate(0);
	if(channelId == -1) {
		printf("msgSendReceiveReply: server: error creating channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	sleep(WAIT);

	while(run) {
		sleep(WAIT_SHORT);

		receiveId = MsgReceive(channelId, msgBuffer, sizeof(msgBuffer), NULL);
		if(receiveId == -1) {
			printf("msgSendReceiveReply: server: error receiving message!\n");fflush(stdout);
			exit(EXIT_FAILURE);
		}

		printf("Server: receiving message: %s\n", msgBuffer);fflush(stdout);

		strcpy(msgBuffer, "Got the message");
		MsgReply(receiveId, EOK, msgBuffer, sizeof(msgBuffer));
		if(receiveId == -1) {
			printf("msgSendReceiveReply: server: error replying message!\n");fflush(stdout);
			exit(EXIT_FAILURE);
		}
	}

	res = ChannelDestroy(channelId);
	if(res == -1) {
		printf("msgSendReceiveReply: server: error destroying channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);
}

static void* client(void* arg) {
	int nodeId = 0; // 0 = local
	pid_t pid = 0; // 0 = same process
	int connectionId = 0;
	char send_msg[] = "Hello World!";
	char receive_msg[512];
	int res = 0;

	connectionId = ConnectAttach(nodeId, pid, channelId, 0, 0);
	if(connectionId == -1) {
		printf("msgSendReceiveReply: client: error attaching to channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	while(run) {
		res = MsgSend(connectionId, &send_msg, strlen(send_msg)+1, receive_msg, sizeof(receive_msg));
		if(res == -1) {
			printf("msgSendReceiveReply: client: error during sending message!\n");fflush(stdout);
			exit(EXIT_FAILURE);
		}

		printf("Client: receiving answer: %s\n", receive_msg);fflush(stdout);
	}

	res = ConnectDetach(connectionId);
	if(res == -1) {
		printf("msgSendReceiveReply: client: error detaching from channel!\n");fflush(stdout);
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);
}

void msgSendReceiveReply_start() {
	printf("running msgSendReceiveReply_start()\n");fflush(stdout);

	pthread_create(&t1, NULL, &server, NULL);
	pthread_create(&t2, NULL, &client, NULL);

	sleep(RUNTIME);

	run = false;

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
