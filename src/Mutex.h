#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>

class Mutex {
public:

	Mutex();
	virtual ~Mutex();
	void lock();
	void unlock();

private:
	pthread_mutex_t mutex;
};

#endif /* MUTEX_H_ */

