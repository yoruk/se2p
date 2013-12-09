#ifndef SERIALCOM_H_
#define SERIALCOM_H_

class SerialCom {
	public:
		~SerialCom();
		static SerialCom* getInstance();
		int start_pkg_receiver(int Chid);
		int send_puk_data_pkg(int puk_id, int puk_type, int puk_height);
		int send_msg_pkg(int msg_content);

	private:
		SerialCom();
};

#endif /* SERIALCOM_H_ */



