#include "Test_Dispatcher.h"
#include "hw.h"
void test_Dispatcher(){
    cout << "Dispatcher" << endl;
    cout << "=========================================================" << endl;
    cout << endl;

    Dispatcher* disp = Dispatcher::getInstance();
    Sensorik* sens = Sensorik::getInstance();
    disp->start(NULL);
    sens->start(NULL);

    string quit;
    do {
        cin >> quit;
    } while (quit != "q");

    cout << "Quitting...";

    disp->stop();
    disp->join();
    sens->stop();
    sens->join();
    reset_HW();
    cout << " done." << endl;
}
