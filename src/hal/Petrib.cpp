#include "Petrib.h"
#include "Petriheader.h"

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petrib* petri; /// the Petri_Bsp object itself

bool places[N_PLACE];

Petrib::Petrib() {
	Conveyor* conv = Conveyor::getInstance();
	Gate* gate = Gate::getInstance();
	TrafficLight* trafficlight = TrafficLight::getInstance();
	Led* led = Led::getInstance();
}

Petrib::~Petrib() {
	delete petri;
	petri = NULL;
}


// returns the only running instance of Petri_Bsp
Petrib* Petrib::getInstance() {

	mutex->lock();

	if (!init_HW_Done()) {
		init_HW();
	}

	if (!petri) {
		petri = new Petrib();
	}

	mutex->unlock();

	return petri;
}

void init_places(){
	places[0] = 1;
	places[1] = 0;
	places[2] = 0;
	places[3] = 0;
	places[4] = 0;
	places[5] = 0;
}

void read_inputs(int code, int value){
	switch (code) {
		case 2:
			if (((value & BIT_0) == 0) && !inputs[EINLAUF_WERKSTUECK]) {
				std::cout << "LS-Eingang unterbrochen" << std::endl;
				inputs[EINLAUF_WERKSTUECK] = true;
			} else if ((value & BIT_0) && inputs[EINLAUF_WERKSTUECK]) {
				std::cout << "LS-Eingang frei" << std::endl;
				inputs[EINLAUF_WERKSTUECK] = false;
			}

			if (((value & BIT_1) == 0) && !inputs[WERKSTUECK_IN_HOEHENMESSUNG]) {
				std::cout << "LS-Hoehenmessung unterbrochen" << std::endl;
				inputs[WERKSTUECK_IN_HOEHENMESSUNG] = true;
			} else if ((value & BIT_1) && inputs[WERKSTUECK_IN_HOEHENMESSUNG]) {
				std::cout << "LS-Hoehenmessung frei" << std::endl;
				inputs[WERKSTUECK_IN_HOEHENMESSUNG] = false;
			}

			if ((value & BIT_2) && !inputs[HOENMESSUNG]) {
				std::cout << "Werkstueck nicht flach" << std::endl;
				inputs[HOENMESSUNG] = true;
			} else if (((value & BIT_2) == 0) && inputs[HOENMESSUNG]) {
				std::cout << "Werkstueck flach" << std::endl;
				inputs[HOENMESSUNG] = false;
			}

			if (((value & BIT_3) == 0) && !inputs[WERKSTUECK_IN_WEICHE]) {
				std::cout << "LS-Weiche unterbrochen" << std::endl;
				inputs[WERKSTUECK_IN_WEICHE] = true;
			} else if ((value & BIT_3) && inputs[WERKSTUECK_IN_WEICHE]) {
				std::cout << "LS-Weiche frei" << std::endl;
				inputs[WERKSTUECK_IN_WEICHE] = false;
			}

			if ((value & BIT_4) && !inputs[WERKSTUECK_METALL]) {
				std::cout << "Werkstueck Metall" << std::endl;
				inputs[WERKSTUECK_METALL] = true;
			} else if (((value & BIT_4) == 0) && inputs[WERKSTUECK_METALL]) {
				std::cout << "Werkstueck kein Metall" << std::endl;
				inputs[WERKSTUECK_METALL] = false;
			}

			if ((value & BIT_5) && !inputs[WEICHE_OFFEN]) {
				std::cout << "Weiche offen" << std::endl;
				inputs[WEICHE_OFFEN] = true;
			} else if (((value & BIT_5) == 0) && inputs[WEICHE_OFFEN]) {
				std::cout << "Weiche geschlossen" << std::endl;
				inputs[WEICHE_OFFEN] = false;
			}

			if (((value & BIT_6) == 0) && !inputs[RUTSCHE_VOLL]) {
				std::cout << "Rutsche ist voll" << std::endl;
				inputs[RUTSCHE_VOLL] = true;
			} else if ((value & BIT_6) && inputs[RUTSCHE_VOLL]) {
				std::cout << "Rutsche nicht voll" << std::endl;
				inputs[RUTSCHE_VOLL] = false;
			}

			if (((value & BIT_7) == 0) && !inputs[AUSLAUF_WERKSTUECK]) {
				std::cout << "LS-Auslauf unterbrochen" << std::endl;
				inputs[AUSLAUF_WERKSTUECK] = true;
			} else if ((value & BIT_7) && inputs[AUSLAUF_WERKSTUECK]) {
				std::cout << "LS-Auslauf frei" << std::endl;
				inputs[AUSLAUF_WERKSTUECK] = false;
			}
			break;
		case 8:
			if ((value & BIT_4) && !inputs[TASTE_START]) {
				std::cout << "Starttaste gedrueckt" << std::endl;
				inputs[TASTE_START] = true;
			} else if (((value & BIT_4) == 0) && inputs[TASTE_START]) {
				std::cout << "Starttaste nicht gedrueckt" << std::endl;
				inputs[TASTE_START] = false;
			}

			if (((value & BIT_5) == 0) && !inputs[TASTE_STOP]) {
				std::cout << "Stoptaste gedrueckt" << std::endl;
				inputs[TASTE_STOP] = true;
			} else if ((value & BIT_5) && inputs[TASTE_STOP]) {
				std::cout << "Stoptaste nicht gedrueckt" << std::endl;
				inputs[TASTE_STOP] = false;
			}

			if ((value & BIT_6) && !inputs[TASTE_RESET]) {
				std::cout << "Resettaste gedrueckt" << std::endl;
				inputs[TASTE_RESET] = true;
			} else if (((value & BIT_6) == 0) && inputs[TASTE_RESET]) {
				std::cout << "Resettaste nicht gedrueckt" << std::endl;
				inputs[TASTE_RESET] = false;
			}

			if (((value & BIT_7) == 0) && !inputs[TASTE_E_STOP]) {
				std::cout << "E-stop gedrueckt" << std::endl;
				inputs[TASTE_E_STOP] = true;
			} else if ((value & BIT_7) && inputs[TASTE_E_STOP]) {
				std::cout << "E-stop nicht gedrueckt" << std::endl;
				inputs[TASTE_E_STOP] = false;
			}
			break;
		}
}
void process_transitios(){

	/*_________T0_________*/
	if(places[0] && !places[1] && !places[2] && !places[3] && !places[4] && !places[5] &&  inputs[EINLAUF_WERKSTUECK]){

		places[0] = 0;
		places[1] = 1;
		places[2] = 0;
		places[3] = 0;
		places[4] = 0;
		places[5] = 0;

		outputs[MOTOR_RECHTSLAUF] = true;
		outputs[AMPEL_GRUEN] = true;

	}

	/*_________T1_________*/
	if(!places[0] && places[1] && !places[2] && !places[3] && !places[4] && !places[5] && inputs[WERKSTUECK_IN_WEICHE]){

		places[0] = 0;
		places[1] = 0;
		places[2] = 1;
		places[3] = 0;
		places[4] = 0;
		places[5] = 0;

		outputs[WEICHE_AUF] = true;

	}
	/*_________T2_________*/
	if(!places[0] && !places[1] && places[2] && !places[3] && !places[4] && !places[5] && inputs[AUSLAUF_WERKSTUECK]){
		places[0] = 0;
		places[1] = 0;
		places[2] = 0;
		places[3] = 1;
		places[4] = 0;
		places[5] = 0;

		outputs[MOTOR_RECHTSLAUF] = false;
		outputs[MOTOR_LINKSLAUF] = true;
	}
	/*_________T3_________*/
	if(!places[0] && !places[1] && !places[2] && places[3] && !places[4] && !places[5] && inputs[WERKSTUECK_IN_WEICHE]){
		places[0] = 0;
		places[1] = 0;
		places[2] = 0;
		places[3] = 0;
		places[4] = 1;
		places[5] = 0;


	}
	/*_________T4_________*/
	if(!places[0] && !places[1] && !places[2] && !places[3] && places[4] && !places[5] && !inputs[WERKSTUECK_IN_WEICHE]){
		places[0] = 0;
		places[1] = 0;
		places[2] = 0;
		places[3] = 0;
		places[4] = 0;
		places[5] = 1;
		outputs[WEICHE_AUF] = false;
	}
	/*_________T5_________*/
	if(!places[0] && !places[1] && !places[2] && !places[3] && !places[4] && places[5]&& inputs[EINLAUF_WERKSTUECK]){

		places[0] = 0;
		places[1] = 1;
		places[2] = 0;
		places[3] = 0;
		places[4] = 0;
		places[5] = 0;

		outputs[MOTOR_LINKSLAUF] = false;
		outputs[MOTOR_RECHTSLAUF] = true;

	}
}
void calculate_outputs(){
	if(places[0]) {
		outputs[MOTOR_RECHTSLAUF] = false;
		outputs[MOTOR_LINKSLAUF] = false;
		outputs[MOTOR_LANGSAM] = false;
		outputs[MOTOR_STOP] = false;
		outputs[WEICHE_AUF] = false;
		outputs[AMPEL_GRUEN] = false;
		outputs[AMPEL_GELB] = false;
		outputs[AMPEL_ROT] = false;
		outputs[LED_STARTTASTE] = false;
		outputs[LED_RESETTASTE] = false;
		outputs[LED_Q1] = false;
		outputs[LED_Q2] = false;
	}
	if(places[1]) {
	}
	if(places[2]) {
	}
	if(places[3]) {
	}
	if(places[4]) {
	}
	if(places[5]) {
	}

}

void write_outputs(){

	printf("MOTOR_RECHTSLAUF: %s", outputs[MOTOR_RECHTSLAUF] ? "true" : "false");
	printf("MOTOR_LINKSLAUF: %s", outputs[MOTOR_LINKSLAUF] ? "true" : "false");
	printf("MOTOR_LANGSAM: %s", outputs[MOTOR_LANGSAM] ? "true" : "false");
	printf("MOTOR_STOP: %s", outputs[MOTOR_STOP] ? "true" : "false");
	printf("WEICHE_AUF: %s", outputs[WEICHE_AUF] ? "true" : "false");
	printf("AMPEL_GRUEN: %s", outputs[AMPEL_GRUEN] ? "true" : "false");
	printf("AMPEL_GELB: %s", outputs[AMPEL_GELB] ? "true" : "false");
	printf("AMPEL_ROT: %s", outputs[AMPEL_ROT] ? "true" : "false");
	printf("LED_STARTTASTE: %s", outputs[LED_STARTTASTE] ? "true" : "false");
	printf("LED_RESETTASTE: %s", outputs[LED_RESETTASTE] ? "true" : "false");
	printf("LED_Q1: %s", outputs[LED_Q1] ? "true" : "false");
	printf("LED_Q2: %s", outputs[LED_Q2] ? "true" : "false");

}
void NotifyReactor(){}
void print_places(){}

