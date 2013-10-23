#ifndef SERIAL_H_
#define SERIAL_H_

class Serial {
public:
	static Serial* getInstance();
	virtual ~Serial();

	int write_serial(char* buffer, int size, char interface[]);
	int read_serial(char* buffer, int size, char interface[]);

private:
	Serial();
};

#endif /* SERIAL_H_ */
