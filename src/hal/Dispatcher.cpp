#include "Dispatcher.h"
//#include "HWaccess.h"
#include "../Mutex.h"
#include "hw.h"
#include "Global.h"
#include "Sensorik.h"
#include <string.h>
#include "Petri_Controller_1.h"


static Mutex* mutex = new Mutex();
bool inputs[N_IN];
bool outputs[N_OUT];
bool trafficlight_inputs[TRAFFICLIGHT_N_IN];
bool conveyor_inputs[CONVEYOR_N_IN];
bool notaus = false;
Dispatcher* Dispatcher::instance = NULL;
Petri_Controller_1* petri_Controller_1;

Dispatcher::Dispatcher() {
    controller_2_free = true;

//    petri_Controller_1 = Petri_Controller_1::getInstance();

    // create channel for dispatcher
    dispatcher_Chid = ChannelCreate(0);
    if (dispatcher_Chid == -1) {
        perror("Dispatcher: ChannelCreate signalChid failed");
        exit(EXIT_FAILURE);
    }
    //  printf("Dispatcher::  GELBE Chid: %d\n", dispatcher_Chid);
    //  fflush(stdout);
    // attach to signal channel(stellt die verbindung zu dem channel des Prozesses PID)
    dispatcher_Coid
            = ConnectAttach(0, 0, dispatcher_Chid, _NTO_SIDE_CHANNEL, 0);
    if (dispatcher_Coid == -1) {
        perror("Dispatcher: ConnectAttach signalCoid failed");
        exit(EXIT_FAILURE);
    }

    // create channel for trafficlight
    trafficlight_Chid = ChannelCreate(0);
    if (trafficlight_Chid == -1) {
        perror("Dispatcher: Channel Create TrafficLight failed");
        exit(EXIT_FAILURE);
    }
    //  printf("Dispatcher:: GRUEN: Chid %d\n", trafficlight_Chid);
    //  fflush(stdout);
    // attach to signal channel(stellt die verbindung zu dem channel des Prozesses PID)
    trafficlight_Coid = ConnectAttach(0, 0, trafficlight_Chid,
            _NTO_SIDE_CHANNEL, 0);
    if (trafficlight_Coid == -1) {
        perror("Dispatcher: ConnectAttach trafficlight failed");
        exit(EXIT_FAILURE);
    }
    //  printf("Dispatcher:: GRUEN: Coid %d\n", trafficlight_Coid);
    //  fflush(stdout);

    // create channel for conveyor
    conveyor_Chid = ChannelCreate(0);
    if (conveyor_Chid == -1) {
        perror("Dispatcher: Channel Create TrafficLight failed");
        exit(EXIT_FAILURE);
    }
    //  printf("Dispatcher:: ORANGE: Chid %d\n", conveyor_Chid);
    //  fflush(stdout);
    // attach to signal channel(stellt die verbindung zu dem channel des Prozesses PID)
    conveyor_Coid = ConnectAttach(0, 0, conveyor_Chid, _NTO_SIDE_CHANNEL, 0);
    if (conveyor_Coid == -1) {
        perror("Dispatcher: ConnectAttach trafficlight failed");
        exit(EXIT_FAILURE);
    }
    //  printf("Dispatcher:: ORANGE Coid %d\n", conveyor_Coid);
    //  fflush(stdout);

    Sensorik* sen = Sensorik::getInstance();
    sensorik_Chid = sen->getSignalChid();
    disp_serial = SerialCom::getInstance();
    //  printf("Dispatcher:: BLAU: Chid %d\n", sensorik_Chid);
    //  fflush(stdout);
}

Dispatcher::~Dispatcher() {
    delete instance;
    instance = NULL;
    delete disp_serial;
    disp_serial = NULL;
}

Dispatcher* Dispatcher::getInstance() {
    mutex->lock();
    if (!init_HW_Done()) {
        init_HW();
    }

    if (!instance) {
        instance = new Dispatcher();
    }
    mutex->unlock();

    return instance;
}

void Dispatcher::execute(void* arg) {
    struct _pulse pulse;

    while (!isStopped()) {
        if (-1 == MsgReceivePulse(sensorik_Chid, &pulse, sizeof(pulse), NULL)) {
            if (isStopped()) {
                break; // channel destroyed, Thread ending
            }
            perror("SensorCtrl: MsgReceivePulse");
            exit(EXIT_FAILURE);
        }


        //printf("Dispatcher::    code:%d,  value:%d \n", pulse.code, pulse.value.sival_int);
        //      printf("Dispatcher::MesgRecievePulse\n");fflush(stdout);

        if ((pulse.value.sival_int != CONTROLLER_2_BUSY)
                || (pulse.value.sival_int != CONTROLLER_2_FREE)
                ||(pulse.value.sival_int != SERIAL_NOTAUS)
                ||(pulse.value.sival_int != SERIAL_NOTAUS_X)) {

            read_inputs(pulse.code, pulse.value.sival_int);
        }

        if (pulse.code == PA_TRAFFICLIGHT) {
            //printf("----------------------------------------TRAFFICLIGHT------------------------------------------------------\n");
            fflush(stdout);
            //          printf("Dispatcher::GRUEN Coid: %d\n", trafficlight_Coid);
            //          fflush(stdout);
            if (-1 == MsgSendPulse(trafficlight_Coid, SIGEV_PULSE_PRIO_INHERIT,
                    pulse.code, pulse.value.sival_int)) {
                perror("Dispatcher: MsgSendPulse an trafficlight failed\n");
                exit(EXIT_FAILURE);
            }
        } else if (pulse.code == PA_CONVEYOR) {


            printf("Dispatcher:: ORANGE Coid: %d\n", conveyor_Coid); fflush(stdout);

            if (pulse.value.sival_int == SERIAL_NOTAUS) {
                printf("------------------------------------------SERIAL_NOTAUS------------------------------------------------\n");fflush(stdout);

                //petri_Controller_2->timer_PauseAll();
                notaus = true;
                disp_trafficlight->redOn();
                disp_conveyor->conveyorStop();

            } else if (pulse.value.sival_int == SERIAL_NOTAUS_X) {
                printf("------------------------------------------SERIAL_NOTAUS_X------------------------------------------------\n");fflush(stdout);

                //petri_Controller_2->timer_ContinueAll();
                notaus = false;
                disp_conveyor->conveyorContinue();
                disp_trafficlight->redOff();

            } else if (pulse.value.sival_int == CONTROLLER_2_BUSY) {
                printf(
                        "------------------------------------------VON CTRL_2 AN CTRL_1----------------------------BUSY----------------------------\n");
                fflush(stdout);
                controller_2_free = false;
            } else if (pulse.value.sival_int == CONTROLLER_2_FREE) {
                printf(
                        "------------------------------------------VON CTRL_2 AN CTRL_1----------------------------FREE----------------------------\n");
                fflush(stdout);
                controller_2_free = true;
                if (-1 == MsgSendPulse(dispatcher_Coid,
                        SIGEV_PULSE_PRIO_INHERIT, DUMMY_CODE, DUMMY_VALUE)) {
                    perror("Dispatcher: MsgSendPulse an conveyor failed\n");
                    exit(EXIT_FAILURE);
                }

            } else {
                //printf("------------------------------------------CONVEYOR--------------------------------------------------------\n");
                fflush(stdout);
                if (-1 == MsgSendPulse(conveyor_Coid, SIGEV_PULSE_PRIO_INHERIT,
                        pulse.code, pulse.value.sival_int)) {
                    perror("Dispatcher: MsgSendPulse an conveyor failed\n");
                    exit(EXIT_FAILURE);
                }
            }

//      } else if (pulse.code == PULSE_FROM_TIMER) {
//    	  pulse_from_timer = true;
//          printf(
//                  "------------------------------------------PULSE_FROM_TIMER--------------------------------------------------------\n");
//          //fflush(stdout);
//          //printf("Dispatcher:: BLAU Coid: %d\n", dispatcher_Coid);
//          //fflush(stdout);
//          if (-1 == MsgSendPulse(dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT,
//                  pulse.code, pulse.value.sival_int)) {
//              perror("Dispatcher: MsgSendPulse an conveyor failed\n");
//              exit(EXIT_FAILURE);
//          }
        } else if (pulse.code == PULSE_PUK_INFORMATION) {
            //printf("DEBUG:Dispatcher: package with puk information arrived, sending it further to controller\n");
            //fflush(stdout);
            // pulsenachricht an controller weiterleiten
            if (-1 == MsgSendPulse(dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT,
                    pulse.code, pulse.value.sival_int)) {
                perror("Dispatcher: MsgSendPulse\n");
                exit(EXIT_FAILURE);
            }

        } else {
            //printf("Dispatcher:: GELB Coid: %d\n", dispatcher_Coid);
            //fflush(stdout);
            if (-1 == MsgSendPulse(dispatcher_Coid, SIGEV_PULSE_PRIO_INHERIT,
                    pulse.code, pulse.value.sival_int)) {
                perror("Dispatcher: MsgSendPulse\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void Dispatcher::read_inputs(int code, int value) {
    switch (code) {
    case PB_STATUS:
        if (((value & BIT_0) == 0) && !inputs[EINLAUF_WERKSTUECK]) {
            std::cout << "Dispatcher: LS-Eingang unterbrochen" << std::endl;
            inputs[EINLAUF_WERKSTUECK] = true;
        } else if ((value & BIT_0) && inputs[EINLAUF_WERKSTUECK]) {
            std::cout << "Dispatcher: LS-Eingang frei" << std::endl;
            inputs[EINLAUF_WERKSTUECK] = false;
        }

        if (((value & BIT_1) == 0) && !inputs[WERKSTUECK_IN_HOEHENMESSUNG]) {
            std::cout << "Dispatcher: LS-Hoehenmessung unterbrochen"
                    << std::endl;
            inputs[WERKSTUECK_IN_HOEHENMESSUNG] = true;
        } else if ((value & BIT_1) && inputs[WERKSTUECK_IN_HOEHENMESSUNG]) {
            std::cout << "Dispatcher: LS-Hoehenmessung frei" << std::endl;
            inputs[WERKSTUECK_IN_HOEHENMESSUNG] = false;
        }

        if ((value & BIT_2) && !inputs[HOENMESSUNG]) {
            std::cout << "Dispatcher: Werkstueck nicht flach" << std::endl;
            inputs[HOENMESSUNG] = true;
        } else if (((value & BIT_2) == 0) && inputs[HOENMESSUNG]) {
            std::cout << "Werkstueck flach" << std::endl;
            inputs[HOENMESSUNG] = false;
        }

        if (((value & BIT_3) == 0) && !inputs[WERKSTUECK_IN_WEICHE]) {
            std::cout << "Dispatcher: LS-Weiche unterbrochen" << std::endl;
            inputs[WERKSTUECK_IN_WEICHE] = true;
        } else if ((value & BIT_3) && inputs[WERKSTUECK_IN_WEICHE]) {
            std::cout << "LS-Weiche frei" << std::endl;
            inputs[WERKSTUECK_IN_WEICHE] = false;
        }

        if ((value & BIT_4) && !inputs[WERKSTUECK_METALL]) {
            std::cout << "Dispatcher: Werkstueck Metall" << std::endl;
            inputs[WERKSTUECK_METALL] = true;
        } else if (((value & BIT_4) == 0) && inputs[WERKSTUECK_METALL]) {
            std::cout << "Dispatcher: Werkstueck kein Metall" << std::endl;
            inputs[WERKSTUECK_METALL] = false;
        }

        if ((value & BIT_5) && !inputs[WEICHE_OFFEN]) {
            std::cout << "Dispatcher: Weiche offen" << std::endl;
            inputs[WEICHE_OFFEN] = true;
        } else if (((value & BIT_5) == 0) && inputs[WEICHE_OFFEN]) {
            std::cout << "Dispatcher: Weiche geschlossen" << std::endl;
            inputs[WEICHE_OFFEN] = false;
        }

        if (((value & BIT_6) == 0) && !inputs[RUTSCHE_VOLL]) {
            std::cout << "Dispatcher: Rutsche ist voll" << std::endl;
            inputs[RUTSCHE_VOLL] = true;
        } else if ((value & BIT_6) && inputs[RUTSCHE_VOLL]) {
            std::cout << "Dispatcher: Rutsche nicht voll" << std::endl;
            inputs[RUTSCHE_VOLL] = false;
        }

        if (((value & BIT_7) == 0) && !inputs[AUSLAUF_WERKSTUECK]) {
            std::cout << "Dispatcher: LS-Auslauf unterbrochen" << std::endl;
            inputs[AUSLAUF_WERKSTUECK] = true;
        } else if ((value & BIT_7) && inputs[AUSLAUF_WERKSTUECK]) {
            std::cout << "Dispatcher: LS-Auslauf frei" << std::endl;
            inputs[AUSLAUF_WERKSTUECK] = false;
        }
        break;
    case PC_STATUS:
        if ((value & BIT_4) && !inputs[TASTE_START]) {
            std::cout << "Dispatcher: Starttaste gedrueckt" << std::endl;
            inputs[TASTE_START] = true;
        } else if (((value & BIT_4) == 0) && inputs[TASTE_START]) {
            std::cout << "Dispatcher: Starttaste nicht gedrueckt" << std::endl;
            inputs[TASTE_START] = false;
        }

        if (((value & BIT_5) == 0) && !inputs[TASTE_STOP]) {
            std::cout << "Dispatcher: Stoptaste gedrueckt" << std::endl;
            inputs[TASTE_STOP] = true;
        } else if ((value & BIT_5) && inputs[TASTE_STOP]) {
            std::cout << "Dispatcher: Stoptaste nicht gedrueckt" << std::endl;
            inputs[TASTE_STOP] = false;
        }

        if ((value & BIT_6) && !inputs[TASTE_RESET]) {
            std::cout << "Dispatcher: Resettaste gedrueckt" << std::endl;
            inputs[TASTE_RESET] = true;
        } else if (((value & BIT_6) == 0) && inputs[TASTE_RESET]) {
            std::cout << "Dispatcher: Resettaste nicht gedrueckt" << std::endl;
            inputs[TASTE_RESET] = false;
        }

        if (((value & BIT_7) == 0) && !inputs[TASTE_E_STOP]) {
            std::cout << "Dispatcher: E-stop gedrueckt" << std::endl;

            disp_serial->send_msg_pkg(SERIAL_NOTAUS);
            //petri_Controller_1->timer_ContinueAll();
            notaus = true;

            disp_trafficlight->redOn();
            disp_conveyor->conveyorStop();
            inputs[TASTE_E_STOP] = true;

        } else if ((value & BIT_7) && inputs[TASTE_E_STOP]) {
            std::cout << "Dispatcher: E-stop nicht gedrueckt" << std::endl;

            disp_serial->send_msg_pkg(SERIAL_NOTAUS_X);

            notaus = false;
            disp_conveyor->conveyorContinue();
            disp_trafficlight->redOff();

            inputs[TASTE_E_STOP] = false;
        }
        break;
    case PA_TRAFFICLIGHT:
        if (value == TRAFFICLIGHT_START) {
            std::cout << "Dispatcher: trafficlight_start_signal" << std::endl;
            trafficlight_inputs[TRAFFICLIGHT_START] = true;
        } else {
            trafficlight_inputs[TRAFFICLIGHT_START] = false;
        }

        if (value == TRAFFICLIGHT_YELLOW) {
            std::cout << "Dispatcher: yellow_signal" << std::endl;
            trafficlight_inputs[TRAFFICLIGHT_YELLOW] = true;
        } else {
            trafficlight_inputs[TRAFFICLIGHT_YELLOW] = false;
        }

        if (value == TRAFFICLIGHT_GREEN) {
            std::cout << "Dispatcher: green_signal" << std::endl;
            trafficlight_inputs[TRAFFICLIGHT_GREEN] = true;
        } else {
            trafficlight_inputs[TRAFFICLIGHT_GREEN] = false;
        }

        if (value == TRAFFICLIGHT_RED) {
            std::cout << "Dispatcher: red_signal" << std::endl;
            trafficlight_inputs[TRAFFICLIGHT_RED] = true;
        } else {
            trafficlight_inputs[TRAFFICLIGHT_RED] = false;
        }

        if (value == TRAFFICLIGHT_RED_B) {
            std::cout << "Dispatcher: red_blinkend_signal" << std::endl;
            trafficlight_inputs[TRAFFICLIGHT_RED_B] = true;
        } else {
            trafficlight_inputs[TRAFFICLIGHT_RED_B] = false;
        }
        if (value == TRAFFICLIGHT_END) {
            std::cout << "Dispatcher: END_signal" << std::endl;
            trafficlight_inputs[TRAFFICLIGHT_END] = true;
        } else {
            trafficlight_inputs[TRAFFICLIGHT_END] = false;
        }
        break;

    case PA_CONVEYOR:

        if (value == P_CONVEYOR_START) {
            std::cout << "Dispatcher: conveyor_start_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_START] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_START] = false;
        }

        if (value == P_CONVEYOR_STOP) {
            std::cout << "Dispatcher: stop_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_STOP] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_STOP] = false;
        }

        if (value == P_CONVEYOR_STOP_X) {
            std::cout << "Dispatcher: stop_x_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_STOP_X] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_STOP_X] = false;
        }

        if (value == P_CONVEYOR_SLOW) {
            std::cout << "Dispatcher: slow_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_SLOW] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_SLOW] = false;
        }

        if (value == P_CONVEYOR_SLOW_X) {
            std::cout << "Dispatcher: slow_x_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_SLOW_X] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_SLOW_X] = false;
        }

        if (value == P_CONVEYOR_RIGHT) {
            std::cout << "Dispatcher: right_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_RIGHT] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_RIGHT] = false;
        }

        if (value == P_CONVEYOR_LEFT) {
            std::cout << "Dispatcher: left_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_LEFT] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_LEFT] = false;
        }

        if (value == P_CONVEYOR_NOTAUS) {
            std::cout << "Dispatcher: notaus_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_NOTAUS] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_NOTAUS] = false;
        }

        if (value == P_CONVEYOR_NOTAUS_X) {
            std::cout << "Dispatcher:  notaus_x_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_NOTAUS_X] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_NOTAUS_X] = false;
        }
        if (value == P_CONVEYOR_END) {
            std::cout << "Dispatcher:  END_signal" << std::endl;
            conveyor_inputs[P_CONVEYOR_END] = true;
        } else {
            conveyor_inputs[P_CONVEYOR_END] = false;
        }

        break;
    }
}

void Dispatcher::set_disp_Outputs(bool tmpArr[]) {

    outputs[0] = tmpArr[0];
    outputs[1] = tmpArr[1];
    outputs[2] = tmpArr[2];
    outputs[3] = tmpArr[3];
    outputs[4] = tmpArr[4];
    outputs[5] = tmpArr[5];
    outputs[6] = tmpArr[6];
    outputs[7] = tmpArr[7];
    outputs[8] = tmpArr[8];
    outputs[9] = tmpArr[9];
    outputs[10] = tmpArr[10];
    outputs[11] = tmpArr[11];
    outputs[12] = tmpArr[12];
}

void Dispatcher::set_disp_Inputs(bool tmpArr[]) {
    inputs[0] = tmpArr[0];
    inputs[1] = tmpArr[1];
    inputs[2] = tmpArr[2];
    inputs[3] = tmpArr[3];
    inputs[4] = tmpArr[4];
    inputs[5] = tmpArr[5];
    inputs[6] = tmpArr[6];
    inputs[7] = tmpArr[7];
    inputs[8] = tmpArr[8];
    inputs[9] = tmpArr[9];
    inputs[10] = tmpArr[10];
    inputs[11] = tmpArr[11];
}

bool* Dispatcher::get_disp_Inputs() {
    return inputs;
}

bool* Dispatcher::get_disp_Outputs() {
    return outputs;
}

bool* Dispatcher::get_trafficlight_inputs() {
    return trafficlight_inputs;
}

bool* Dispatcher::get_conveyor_inputs() {
    return conveyor_inputs;
}

void Dispatcher::shutdown() {
}

int Dispatcher::get_disp_Chid() {
    return dispatcher_Chid;
}

int Dispatcher::get_disp_Coid() {
    return dispatcher_Coid;
}
int Dispatcher::get_trafficLight_Chid() {
    return trafficlight_Chid;
}
int Dispatcher::get_conveyor_Chid() {
    return conveyor_Chid;
}
