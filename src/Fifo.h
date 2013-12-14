#ifndef FIFO_H_
#define FIFO_H_

#include "Puk.h"
#include "Mutex.h"
#include "Global.h"

class Fifo {
	public:
		Fifo();
		virtual ~Fifo();
		int put(Puk* p);
		Puk* remove();
		Puk* get();
	private:
		int read_idx;
		int write_idx;
		int count;
		Puk* buffer[BUFFER_LENGTH];
		Mutex* mutex;
		void clear();
};

#endif /* FIFO_H_ */
