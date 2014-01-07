#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "HAWThread.h"
#include "SerialCom.h"
#include "Conveyor.h"
#include "TrafficLight.h"

class Dispatcher: public thread::HAWThread {

public:
    virtual ~Dispatcher();
    static Dispatcher* getInstance();

    bool controller_2_free;

    int get_disp_Chid();
    int get_disp_Coid();
    bool* get_disp_Inputs();
    bool* get_disp_Outputs();
    void set_disp_Inputs(bool tmpArr[]);
    void set_disp_Outputs(bool tmpArr[]);
    void read_inputs(int code, int value);
    bool* get_trafficlight_inputs();
    bool* get_conveyor_inputs();
    int get_trafficLight_Chid();
    int get_conveyor_Chid();
private:
    int sensorik_Chid;

    int dispatcher_Chid;
    int dispatcher_Coid;

    int trafficlight_Chid;
    int trafficlight_Coid;
    int conveyor_Chid;
    int conveyor_Coid;

    static Dispatcher* instance;
    Conveyor* disp_conveyor;
    TrafficLight* disp_trafficlight;
    SerialCom* disp_serial;



    Dispatcher();

protected:
    virtual void execute(void* arg);
    virtual void shutdown();

};

#endif /* DISPATCHER_H_ */
