#ifndef FIFO_H_
#define FIFO_H_

#include "Puk.h"

#define LENGHT 4
#define FIRSTFIELD 0
#define LASTFIELD 3

class Fifo {

public:
	Fifo();
	virtual ~Fifo();
	void init_fifo();
	void put(Puk element);
	Puk remove();
	Puk* get();
private:
	Puk buffer[LENGHT];
};
#endif /* FIFO_H_ */

