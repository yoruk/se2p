#ifndef PETRIHEADER_H_
#define PETRIHEADER_H_

#define N_IN 12
#define N_OUT 12
#define N_PLACE 6

bool inputs[N_IN];
bool outputs[N_OUT];


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

#endif /* PETRIHEADER_H_ */
