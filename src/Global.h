#ifndef GLOBAL_H_
#define GLOBAL_H_

// bools
#define FALSE 0
#define TRUE 1

// IO addresses for geme
#define D_IOBASE 0x300 // Base address
#define DIO_A (D_IOBASE + 0x00) // Port A
#define DIO_B (D_IOBASE + 0x01) // Port B
#define DIO_C (D_IOBASE + 0x02) // Port C
#define DIGITAL_CARD_CONTROL (D_IOBASE + 0x03) // Control register

#define IO_BASE         0x320
#define IO_OFFS_A         0x02
#define IO_GET_VAL 0x10
#define IO_PORT_A         (IO_BASE + IO_OFFS_A)

// bitmasks for single bits
#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80
#define BIT_NONE 0x00
#define BIT_ALL 0xff
#define BIT_FIRST_NIBBLE 0x0f
#define BIT_SECOND_NIBBLE 0xf0

#define OFFS_INT_STATUS 0x0F
#define PB_STATUS       0x02
#define PC_STATUS       0x08

#define OFFS_INT_CTRL   0x0B
#define PB_CTRL         0x02
#define PC_CTRL         0x04

// conveyor
#define CONVEYOR_RIGHT BIT_0
#define CONVEYOR_LEFT BIT_1
#define CONVEYOR_SLOW BIT_2
#define CONVEYOR_STOP BIT_3
#define CONVEYOR_STOPPED 0
#define CONVEYOR_RUNNING_SLOW 1
#define CONVEYOR_RUNNING_FAST 2
#define CONVEYOR_DIRECTION_LEFT 0
#define CONVEYOR_DIRECTION_RIGHT 1

// gate
#define GATE BIT_4

//traffic light
#define TRAFFIC_LIGHT_GREEN BIT_5
#define TRAFFIC_LIGHT_YELLOW BIT_6
#define TRAFFIC_LIGHT_RED BIT_7

// led
#define LED_START_BUTTON BIT_0
#define LED_RESET_BUTTON BIT_1
#define LEDD_Q1 BIT_2
#define LEDD_Q2 BIT_3

// serial interface
#define SERIAL_INTERFACE "/dev/ser1"
#define SERIAL_SPEED B9600

#define N_IN 12
#define N_OUT 13


/*---------INPUTS----------*/
#define EINLAUF_WERKSTUECK 0
#define WERKSTUECK_IN_HOEHENMESSUNG 1
#define HOENMESSUNG 2
#define WERKSTUECK_IN_WEICHE 3
#define WERKSTUECK_METALL 4
#define WEICHE_OFFEN 5
#define RUTSCHE_VOLL 6
#define AUSLAUF_WERKSTUECK 7
#define TASTE_START 8
#define TASTE_STOP 9
#define TASTE_RESET 10
#define TASTE_E_STOP 11

/*---------OUTPUTS---------*/
#define MOTOR_RECHTSLAUF 0
#define MOTOR_LINKSLAUF 1
#define MOTOR_LANGSAM 2
#define MOTOR_STOP 3
#define WEICHE_AUF 4
#define AMPEL_GRUEN 5
#define AMPEL_GELB 6
#define AMPEL_ROT 7
#define LED_STARTTASTE 8
#define LED_RESETTASTE 9
#define LED_Q1 10
#define LED_Q2 11
#define AMPEL_ROT_B 12

/*---------Conveyor_INPUTS-----------*/
#define CONVEYOR_N_IN 10

#define P_CONVEYOR_START 0
#define P_CONVEYOR_STOP 1
#define P_CONVEYOR_STOP_X 2
#define P_CONVEYOR_SLOW 3
#define P_CONVEYOR_SLOW_X 4
#define P_CONVEYOR_RIGHT 5
#define P_CONVEYOR_LEFT 6
#define P_CONVEYOR_NOTAUS 7
#define P_CONVEYOR_NOTAUS_X 8
#define P_CONVEYOR_END 9

/*----------trafficlight_INPUTS----------*/
#define TRAFFICLIGHT_N_IN 6

#define TRAFFICLIGHT_START 0
#define TRAFFICLIGHT_YELLOW 1
#define TRAFFICLIGHT_GREEN 2
#define TRAFFICLIGHT_RED 3
#define TRAFFICLIGHT_RED_B 4
#define TRAFFICLIGHT_END 5

/*-------Timer values--------*/
#define TIMER_FULL 0
#define TIMER_GATE 1

/*-------STATUS CONTROLLER 2 values--------*/
#define CONTROLLER_2_FREE 0
#define CONTROLLER_2_BUSY 1

/*--------Times-----------*/

#define C1_CLOSE_GATE_TIME 1
#define C1_KICK_TIME 50
#define SLIDE_FULL_TIME 2
#define C1_PUK_LOST_TIME 4
#define C1_MANY_PUKS_TIME 4
#define C2_PUK_LOST_TIME 3
#define C2_MANY_PUKS_TIME 3
#define C2_CLOSE_GATE_TIME 1

// Pulse
#define PULSE_FROM_TIMER 60
#define PA_TRAFFICLIGHT	70
#define PA_CONVEYOR	80
#define PULSE_PUK_INFORMATION 10
#define PULSE_MSG_FROM_CONVEYOR 20
#define PULSE_OTHER_MACHINE_STATUS PA_CONVEYOR
#define PULSE_FROM_CONTROLLER_2 90
//#define PULSE_CONVEYOR_OK 30
//#define PULSE_CONVEYOR_ESTOP 40


/*--------PUK_BUFFER-----------*/
#define BUFFER_LENGTH 4

/*--------PUK_TYPE-----------*/
#define PUK_LOCH 1
#define PUK_METALL 2
#define PUK_GROSS 3
#define PUK_FLACH 4

/*--------PUK value bitlengths and masks-----------*/
#define PUK_ID_LENGTH 14
#define PUK_TYPE_LENGTH 2
#define PUK_HEIGHT_LENGTH 16
#define PUK_ID_MASK 0x3FFF // lowest 12 bit
#define PUK_TYPE_MASK 0x3 // lowest 2 bit
#define PUK_HEIGHT_MASK 0xFFFF // lowest 16 bit

#endif /* GLOBAL_H_ */
