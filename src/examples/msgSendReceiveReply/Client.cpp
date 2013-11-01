#include <iostream>
#include <unistd.h>
#include <errno.h>
#include "msgSendReceiveReply.h"
#include "Client.h"

extern int channelId;

Client::Client() {
	// TODO Auto-generated constructor stub
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

void Client::stop() {
	HAWThread::stop(); // super.stop();

	ConnectDetach(connectionId);
}

void Client::shutdown() {
	cout << "Client: Shutting down..." << endl;
}

void Client::execute(void *arg) {
	int nodeId = 0; // local
	pid_t pid = 0; // local
	char send_msg[] = "Hello World!";
	char receive_msg[512];

	connectionId = ConnectAttach(nodeId, pid, channelId, 0, 0);

	while(!isStopped()) {
		//cout << "Client: sending message: " << send_msg << endl;
		MsgSend(connectionId, &send_msg, strlen(send_msg)+1, receive_msg, sizeof(receive_msg));

		cout << "Client: receiving answer: " << receive_msg << endl;

		sleep(1);
	}
}
