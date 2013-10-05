
#include <cstdlib>
#include <iostream>
#include<unistd.h>
#include <errno.h>
#include "ThreadAmpel.h"
#include "HWaccess.h"
#include "defines.h"

using namespace std;
using namespace thread;

int main(int argc, char *argv[]) {
	std::cout << "Welcome to the QNX Momentics IDE" << std::endl;

#ifdef SIMULATION
	cout << "Simulation aktiv" << endl;
	cout << "Zum Aufbau der Verbindung muss Die Festo Simulation schon laufen."
			<< endl;
	IOaccess_open(); // Baue die Verbindung zur Simulation auf
#endif

	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("ThreadCtl access failed\n");
		return -1;
	}
	// Initialisierung der Digitalen IO Karte
	out8(DIO_BASE + DIO_OFFS_CTRL, 0x8A);

	ThreadAmpel thread;

	thread.start(NULL);
	sleep(5);
	thread.stop();
	thread.join();

	cout << "Test beendet" << endl;
#ifdef SIMULATION
	IOaccess_close(); // Schließe die Verbindung zur Simulation
#endif
	return EXIT_SUCCESS;
}
