/*
 * ThreadAmpel.h
 *
 *  Created on: 5 Oct 2013
 *
 */

#ifndef THREADAMPEL_H_
#define THREADAMPEL_H_
#include "HWaccess.h"
#include "HAWThread.h"

namespace thread {

class ThreadAmpel: public HAWThread {
public:
	ThreadAmpel();
	virtual ~ThreadAmpel();
	virtual void execute(void*);
	virtual void shutdown();
};
}
#endif /* THREADAMPEL_H_ */
