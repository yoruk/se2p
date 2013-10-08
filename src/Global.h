#ifndef GLOBAL_H_
#define GLOBAL_H_

// IO addresses for geme
#define D_IOBASE 0x300 // Base address
#define DIO_A (D_IOBASE + 0x00) // Port A
#define DIO_B (D_IOBASE + 0x01) // Port B
#define DIO_C (D_IOBASE + 0x02) // Port C
#define DIGITAL_CARD_CONTROL (D_IOBASE + 0x03) // Control register

// conveyer belt
#define RIGHT 0x01
#define LEFT 0x02
#define SLOW 0x04
#define STOP 0x08

// gate
#define GATE 0x10

// traffic light
#define GREEN 0x20
#define YELLOW 0x40
#define RED 0x80

// bitmasks for single bits
#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80

#endif /* GLOBAL_H_ */
