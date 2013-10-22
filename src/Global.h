#ifndef GLOBAL_H_
#define GLOBAL_H_

// IO addresses for geme
#define D_IOBASE 0x300 // Base address
#define DIO_A (D_IOBASE + 0x00) // Port A
#define DIO_B (D_IOBASE + 0x01) // Port B
#define DIO_C (D_IOBASE + 0x02) // Port C
#define DIGITAL_CARD_CONTROL (D_IOBASE + 0x03) // Control register

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

// traffic light
#define TRAFFIC_LIGHT_GREEN BIT_5
#define TRAFFIC_LIGHT_YELLOW BIT_6
#define TRAFFIC_LIGHT_RED BIT_7

// led
#define LED_START_BUTTON BIT_0
#define LED_RESET_BUTTON BIT_1
#define LED_Q1 BIT_2
#define LED_Q2 BIT_3

// serial interface
#define SERIAL_INTERFACE_1 "/dev/ser1"
#define SERIAL_INTERFACE_2 "/dev/ser2"

#endif /* GLOBAL_H_ */
