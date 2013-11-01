#include <iostream>
#include <unistd.h>
#include <errno.h>
#include "messageSendReceiveReply.h"
#include "Server.h"
#include "sys/neutrino.h"

int channelId = -1;

Server::Server() {
	// TODO Auto-generated constructor stub
}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

void Server::stop() {
	HAWThread::stop(); // super.stop();

	ChannelDestroy(channelId);
}

void Server::shutdown() {
	cout << "Server: Shutting down..." << endl;
}

void Server::execute(void *arg) {
	int receiveId;
	char msgBuffer[256];

	channelId = ChannelCreate(0);

	while(!isStopped()) {
		//cout << "Server: is waiting for message" << endl;

		receiveId = MsgReceive(channelId, msgBuffer, sizeof(msgBuffer), NULL);
		cout << "Server: receiving message: " << msgBuffer << endl;

		strcpy(msgBuffer, "Got the message");

		MsgReply(receiveId, EOK, msgBuffer, sizeof(msgBuffer));
	}
}
