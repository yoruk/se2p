#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "msgSndRcvRpl.h"
#include "Server.h"
#include "Client.h"

int channelId = -1;

void msgSndRcvRpl_start() {
	printf("running msgSndRcvRpl_start()\n");fflush(stdout);

	Server* server = new Server();
	Client* client = new Client();

	server->start(NULL);
	client->start(NULL);

	sleep(20);

	client->stop();
	server->stop();
}
