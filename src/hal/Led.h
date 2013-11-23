#ifndef LED_H_
#define LED_H_

#include "../Mutex.h"
#include "Util.h"
#include "hw.h"

class Led {
public:
	static Led* getInstance();
    virtual ~Led();

    void led_StartButton_On();
    void led_StartButton_Off();
    void led_ResetButton_On();
    void led_ResetButton_Off();
    void led_Q1_On();
    void led_Q1_Off();
    void led_Q2_On();
    void led_Q2_Off();

private:
    Led();
};

#endif /* LED_H_ */
