#ifndef PUK_H_
#define PUK_H_

class Puk {
public:
	Puk();
	virtual ~Puk();
	//Getter
	int get_id();
	char get_typ();
	int get_hoehenmessung ();
	bool get_status_flach();

	//Setter
	void set_typ(char par);
	void set_hoehenmessung(int par);
	void set_status_flach(bool par);

private:
	static int count;
	int id;
	char typ;
	int hoehenmessung;
	bool status_flach;
};

#endif /* PUK_H_ */
