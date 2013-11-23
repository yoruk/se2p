#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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

