#ifndef SERIAL_H_
#define SERIAL_H_

#include <termios.h>

class Serial {
	public:
		~Serial();
		static Serial* getInstance();
		int open_serial();
		int close_serial();
		int write_serial(unsigned char* buffer, int size);
		int read_serial(unsigned char* buffer, int size);

	private:
		int init_done;
		int fd;
		struct termios new_port_settings;
		struct termios old_port_settings;

		Serial();
};

#endif /* SERIAL_H_ */
