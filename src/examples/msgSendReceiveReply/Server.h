#ifndef SERVER_H_
#define SERVER_H_

#include "../../HAWThread.h"

class Server: public thread::HAWThread {
	public:
		Server();
		virtual ~Server();
		void stop();

	protected:
		virtual void execute(void* arg);
		virtual void shutdown();
};

#endif /* SERVER_H_ */
