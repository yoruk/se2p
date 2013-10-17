#include <stdlib.h>
#include <stdio.h>
#include "Test_Util.h"
#include "../hal/Util.h"
#include "../Global.h"

void test_Util_start() {
	unsigned char reg = 0;

	printf("Test_Util/test_Util_start()\n\n");
	printf("reg: 0x%x\n\n", reg);fflush(stdout);

	printf("Bit 1 setzen\n");fflush(stdout);
	setBit(&reg, BIT_0);
	printf("Ist Bit 1 gesetzt? ");fflush(stdout);
	if(bitIsSet(&reg, BIT_0)) {
		printf("Ja\n");fflush(stdout);
	} else {
		printf("Nein\n");fflush(stdout);
	}
	printf("reg: 0x%x\n\n", reg);fflush(stdout);

	printf("Bit 1 loeschen\n");fflush(stdout);
	unsetBit(&reg, BIT_0);
	printf("Ist Bit 1 gesetzt? ");fflush(stdout);
	if(bitIsSet(&reg, BIT_0)) {
		printf("Ja\n");fflush(stdout);
	} else {
		printf("Nein\n");fflush(stdout);
	}
	printf("reg: 0x%x\n\n", reg);fflush(stdout);
}
