#ifndef PUK_H_
#define PUK_H_

#include "Global.h"

class Puk {
public:
	Puk();
	virtual ~Puk();
	//Getter
	int get_id();
	char get_typ();
	int get_hoehenmessung1 ();
	int get_hoehenmessung2 ();
	bool get_status_flach();

	//Setter
	void set_typ(char par);
	void set_hoehenmessung1(int par);
	void set_hoehenmessung2(int par);
	void set_status_flach(bool par);

private:
	static int count;
	int id;
	char typ;
	int hoehenmessung1;
	int hoehenmessung2;
	bool status_flach;
};

#endif /* PUK_H_ */
