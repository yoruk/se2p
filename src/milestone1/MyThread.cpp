#include "MyThread.h"

using namespace std;

namespace thread {
	MyThread::MyThread() {
		// TODO Auto-generated constructor stub
	}

	MyThread::~MyThread() {
		// TODO Auto-generated destructor stub
	}

	void MyThread::shutdown() {
		cout << "Shutting down..." << endl;
	}

	void MyThread::execute(void *arg) {
		while(!isStopped()) {
			cout << "red" << endl;
			out8(D_IOBASE, TRAFFIC_LIGHT_RED);
			sleep(1);

			cout << "yellow" << endl;
			out8(D_IOBASE, TRAFFIC_LIGHT_YELLOW);
			sleep(1);

			cout << "green" << endl;
			out8(D_IOBASE, TRAFFIC_LIGHT_GREEN);
			sleep(1);
		}
	}
}
