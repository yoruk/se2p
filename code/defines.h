/*
 * defines.h
 *
 *  Created on: 5 Oct 2013
 *
 */

#ifndef DEFINES_H_
#define DEFINES_H_


#define DIO_BASE 		0x300        // Base address
#define DIO_OFFS_A 		0x00         // Port A offset
#define DIO_OFFS_B 		0x01         // Port B offset
#define DIO_OFFS_C 		0x02         // Port C offset
#define DIO_OFFS_CTRL 	0x03      	 // Control register offset






#define RECHTS 		0x01		//Motor Rechtslauf
#define LINKS		0x02		//Motor Linkslauf
#define SLOW 		0x04		//Motor Langsam (nicht benoetigt)
#define STOP 		0x08		//Motor stopp
#define WEICHE_AUF 	0x10		//Weiche Auf
#define GRUEN 		0x20		//Grün
#define GELB 		0x40		//Gelb
#define ROT 		0x80		//Rot


#define BIT_0 		0x01
#define BIT_1 		0x02
#define BIT_2 		0x04
#define BIT_3 		0x08
#define BIT_4 		0x10
#define BIT_5 		0x20
#define BIT_6 		0x40
#define BIT_7 		0x80


#endif /* DEFINES_H_ */
