/*
 * Test_Sensorik.cpp
 *
 *  Created on: 05.11.2013
 *      Author: Admin
 */

#include "Test_Sensorik.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <Sensorik.h>

using namespace std;
using namespace hal;

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
