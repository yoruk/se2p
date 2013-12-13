#include "Test_Dispatcher.h"
#include "hw.h"
void test_Dispatcher(){
    cout << "Dispatcher" << endl;
    cout << "=========================================================" << endl;
    cout << endl;

//    Petri_Controller_1* petri_controller_1 = Petri_Controller_1::getInstance();
    Petri_Controller_2* petri_controller_2 = Petri_Controller_2::getInstance();
    Petri_Conveyor* petri_conveyor = Petri_Conveyor::getInstance();
    Petri_TrafficLight* petri_trafficlight = Petri_TrafficLight::getInstance();
    Dispatcher* disp = Dispatcher::getInstance();
    Sensorik* sens = Sensorik::getInstance();
    //SerialCom* serialCom = SerialCom::getInstance();
    //serialCom->startReceiver(sens->getSignalChid());

    petri_conveyor->start(NULL);
    petri_trafficlight->start(NULL);
    //petri_controller_1->start(NULL);
    petri_controller_2->start(NULL);
    disp->start(NULL);
    sens->start(NULL);


    string quit;
    do {
        cin >> quit;

       cout << quit << endl;
    } while (quit != "q");

    cout << "Quitting...";
    petri_conveyor->stop();
    petri_conveyor->join();
    petri_trafficlight->stop();
    petri_trafficlight->join();
//    petri_controller_1->stop();
//    petri_controller_1->join();
    petri_controller_2->stop();
    petri_controller_2->join();
    disp->stop();
    disp->join();
    sens->stop();
    sens->join();

    reset_HW();
    cout << " done." << endl;
}
