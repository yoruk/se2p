#include "Test_Sensorik.h"

void test_Sensorik(){
    cout << "Sensorik" << endl;
    cout << "=========================================================" << endl;
    cout << endl;

    Sensorik* sens = Sensorik::getInstance();
    sens->start(NULL);

    string quit;
    do {
        cin >> quit;
    } while (quit != "q");

    cout << "Quitting...";

    sens->stop();
    sens->join();

    cout << " done." << endl;
}
