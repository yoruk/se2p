#include "Petrib.h"

static Mutex* mutex = new Mutex(); /// the mutex for controlling the access
static Petrib* petri; /// the Petri_Bsp object itself
void init_places();
bool inputs[N_IN];
bool outputs[N_OUT];

bool places[N_PLACE];

Petrib::Petrib() {
	conv = Conveyor::getInstance();
	gate = Gate::getInstance();
	trafficlight = TrafficLight::getInstance();
	led = Led::getInstance();
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

void Petrib::init_places() {
	places[0] = true;
	places[1] = false;
	places[2] = false;
	places[3] = false;
	places[4] = false;
	places[5] = false;
}

void Petrib::read_inputs(int code, int value) {
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
void Petrib::process_transitios() {

	/*_________T0_________*/
	if (places[0] && !places[1] && !places[2] && !places[3] && !places[4]
			&& !places[5] && inputs[EINLAUF_WERKSTUECK]) {

		places[0] = 0;
		places[1] = 1;
		places[2] = 0;
		places[3] = 0;
		places[4] = 0;
		places[5] = 0;

		outputs[MOTOR_RECHTSLAUF] = true;
		outputs[AMPEL_GRUEN] = true;
		puts("T0 MOTOR_RECHTSLAUF AUF.....................................................\n");
		puts("T0 AMPEL_GRUEN.....................................................\n");

	}

	/*_________T1_________*/
	if (!places[0] && places[1] && !places[2] && !places[3] && !places[4]
			&& !places[5]) {

		if (inputs[WERKSTUECK_IN_WEICHE]) {
			places[0] = 0;
			places[1] = 0;
			places[2] = 1;
			places[3] = 0;
			places[4] = 0;
			places[5] = 0;
			puts("T1 WEICHE AUF.....................................................\n");
			//gate->open();
			outputs[WEICHE_AUF] = true;
		}
	}
	/*_________T2_________*/
	if (!places[0] && !places[1] && places[2] && !places[3] && !places[4]
			&& !places[5] && inputs[AUSLAUF_WERKSTUECK]) {
		places[0] = 0;
		places[1] = 0;
		places[2] = 0;
		places[3] = 1;
		places[4] = 0;
		places[5] = 0;

		outputs[MOTOR_RECHTSLAUF] = false;
		outputs[MOTOR_LINKSLAUF] = true;
		puts("T2 MOTOR_RECHTSLAUF.....................................................\n");
		puts("T2 MOTOR_LINKSLAUF.....................................................\n");

	}
	/*_________T3_________*/
	if (!places[0] && !places[1] && !places[2] && places[3] && !places[4]
			&& !places[5] && inputs[WERKSTUECK_IN_WEICHE]) {
		places[0] = 0;
		places[1] = 0;
		places[2] = 0;
		places[3] = 0;
		places[4] = 1;
		places[5] = 0;
		puts("T3 .....................................................\n");

	}
	/*_________T4_________*/
	if (!places[0] && !places[1] && !places[2] && !places[3] && places[4]
			&& !places[5] && !inputs[WERKSTUECK_IN_WEICHE]) {
		places[0] = 0;
		places[1] = 0;
		places[2] = 0;
		places[3] = 0;
		places[4] = 0;
		places[5] = 1;
		outputs[WEICHE_AUF] = false;
		puts("T4 WEICHE_AUF.....................................................\n");
	}
	/*_________T5_________*/
	if (!places[0] && !places[1] && !places[2] && !places[3] && !places[4]
			&& places[5] && inputs[EINLAUF_WERKSTUECK]) {

		places[0] = 0;
		places[1] = 1;
		places[2] = 0;
		places[3] = 0;
		places[4] = 0;
		places[5] = 0;

		outputs[MOTOR_LINKSLAUF] = false;
		outputs[MOTOR_RECHTSLAUF] = true;
		//conv->moveLeft();
		puts("T5 MOTOR_LINKSLAUF.....................................................\n");
		puts("T5 MOTOR_RECHTSLAUF.....................................................\n");

	}
}
void Petrib::calculate_outputs() {
	if (places[0]) {
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
//	if (places[1]) {
//	}
//	if (places[2]) {
//	}
//	if (places[3]) {
//	}
//	if (places[4]) {
//	}
//	if (places[5]) {
//	}

}

void Petrib::write_outputs() {
	puts("_________________________________________________________________ \n");
	printf("MOTOR_RECHTSLAUF: %s\n", outputs[MOTOR_RECHTSLAUF] ? "true"
			: "false");
	fflush(stdout);
	printf("MOTOR_LINKSLAUF: %s\n", outputs[MOTOR_LINKSLAUF] ? "true" : "false");
	fflush(stdout);
	printf("MOTOR_LANGSAM: %s\n", outputs[MOTOR_LANGSAM] ? "true" : "false");
	fflush(stdout);
	printf("MOTOR_STOP: %s\n", outputs[MOTOR_STOP] ? "true" : "false");
	fflush(stdout);
	printf("WEICHE_AUF: %s\n", outputs[WEICHE_AUF] ? "true" : "false");
	fflush(stdout);
	printf("AMPEL_GRUEN: %s\n", outputs[AMPEL_GRUEN] ? "true" : "false");
	fflush(stdout);
	printf("AMPEL_GELB: %s\n", outputs[AMPEL_GELB] ? "true" : "false");
	fflush(stdout);
	printf("AMPEL_ROT: %s\n", outputs[AMPEL_ROT] ? "true" : "false");
	fflush(stdout);
	printf("LED_STARTTASTE: %s\n", outputs[LED_STARTTASTE] ? "true" : "false");
	fflush(stdout);
	printf("LED_RESETTASTE: %s\n", outputs[LED_RESETTASTE] ? "true" : "false");
	fflush(stdout);
	printf("LED_Q1: %s\n", outputs[LED_Q1] ? "true" : "false");
	fflush(stdout);
	printf("LED_Q2: %s\n", outputs[LED_Q2] ? "true" : "false");
	fflush(stdout);

}
void Petrib::NotifyReactor() {

	if (outputs[MOTOR_RECHTSLAUF]) {
		conv->moveRight();
	}
	if (outputs[MOTOR_LINKSLAUF]) {
		conv->moveLeft();
	}
	if (outputs[MOTOR_LANGSAM]) {
		conv->moveSlow();
	} else {
		conv->moveFast();
	}
	if (outputs[MOTOR_STOP]) {
		conv->conveyorStop();
	} else {
		conv->conveyorContinue();
	}
	if (outputs[WEICHE_AUF]) {

		gate->open();
	} else {
		gate->close();
	}
	if (outputs[AMPEL_GRUEN]) {
		trafficlight->greenOn();
	} else {
		trafficlight->greenOff();
	}
	if (outputs[AMPEL_GELB]) {
		trafficlight->yellowOn();
	} else {
		trafficlight->yellowOff();
	}
	if (outputs[AMPEL_ROT]) {
		trafficlight->redOn();
	} else {
		trafficlight->redOff();
	}
	if (outputs[LED_STARTTASTE]) {
		led->led_StartButton_On();
	} else {
		led->led_StartButton_Off();
	}
	if (outputs[LED_RESETTASTE]) {
		led->led_ResetButton_On();
	} else {
		led->led_ResetButton_Off();
	}
	if (outputs[LED_Q1]) {
		led->led_Q1_On();
	} else {
		led->led_Q1_Off();
	}
	if (outputs[LED_Q2]) {
		led->led_Q2_On();
	} else {
		led->led_Q2_Off();
	}
}
void Petrib::print_places() {

}

