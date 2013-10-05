/*
 * ThreadAmpel.cpp
 *
 *  Created on: 5 Oct 2013
 *
 */
#include <iostream>
#include <unistd.h>
#include "ThreadAmpel.h"
#include "HWaccess.h"
#include "HAWThread.h"
#include "defines.h"

using namespace std;

namespace thread {

ThreadAmpel::ThreadAmpel() {
	//TODO
}

ThreadAmpel::~ThreadAmpel() {
	// TODO Auto-generated destructor stub
}
void ThreadAmpel::execute(void*) {
	while (!isStopped()) {

		for (int count = 0; count < 5; count++) {
			cout << "rot" << endl;
			out8(DIO_BASE, ROT);//rot
			sleep(1);

			cout << "gelb" << endl;
			out8(DIO_BASE, GELB);//gelb
			sleep(1);

			cout << "gruen" << endl;
			out8(DIO_BASE, GRUEN);//gruen
			sleep(1);

		}
		out8(DIO_BASE, 0x00);

	}
}

void ThreadAmpel::shutdown() {
	cout << "SHUTDOWN" << endl;
}

}
