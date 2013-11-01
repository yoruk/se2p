#ifndef CLIENT_H_
#define CLIENT_H_

#include "../../HAWThread.h"

class Client: public thread::HAWThread {
	public:
		Client();
		virtual ~Client();
		void stop();

	protected:
		virtual void execute(void* arg);
		virtual void shutdown();

	private:
		int connectionId;
};

#endif /* CLIENT_H_ */
