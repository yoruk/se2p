#ifndef SERIALCOM_H_
#define SERIALCOM_H_

#include <HAWThread.h>

class SerialCom: public thread::HAWThread {
	public:
		~SerialCom();
		static SerialCom* getInstance();
		int send_puk_data_pkg(int puk_id, int puk_type, int puk_height);
		int send_msg_pkg(int msg_content);
		int set_Channel(int chid);

	private:
		SerialCom();

	protected:
		virtual void execute(void* arg);
		virtual void shutdown();
};

#endif /* SERIALCOM_H_ */



