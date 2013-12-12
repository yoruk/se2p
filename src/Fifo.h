#ifndef FIFO_H_
#define FIFO_H_

#include "Puk.h"
#include "Global.h"

#define FIRSTFIELD 0
#define LASTFIELD 3

class Fifo {

public:
	Fifo();
	virtual ~Fifo();
	void put(Puk element);
	Puk remove();
	Puk* get();
private:
	Puk buffer[BUFFER_LENGHT];
};
#endif /* FIFO_H_ */

