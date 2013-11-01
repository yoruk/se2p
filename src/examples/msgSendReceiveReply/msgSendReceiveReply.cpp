#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "msgSendReceiveReply.h"
#include "Server.h"
#include "Client.h"

void msgSendReceiveReply_start() {
	printf("running messageSendReceiveReply_start()\n");fflush(stdout);

	Server* server = new Server();
	Client* client = new Client();

	server->start(NULL);
	client->start(NULL);

	sleep(20);

	client->stop();
	server->stop();
}
