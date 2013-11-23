#include "Test_Util.h"

void test_Util_start() {
	unsigned char reg = 0;

	printf("Test_Util/test_Util_start()\n\n");
	printf("reg: 0x%x\n\n", reg);fflush(stdout);

	printf("set Bit 1\n");fflush(stdout);
	setBit(&reg, BIT_0);
	printf("Is Bit 1 set? ");fflush(stdout);
	if(bitIsSet(&reg, BIT_0)) {
		printf("Yes\n");fflush(stdout);
	} else {
		printf("No\n");fflush(stdout);
	}
	printf("reg: 0x%x\n\n", reg);fflush(stdout);

	printf("delete Bit 1\n");fflush(stdout);
	unsetBit(&reg, BIT_0);
	printf("Is Bit 1 set? ");fflush(stdout);
	if(bitIsSet(&reg, BIT_0)) {
		printf("Yes\n");fflush(stdout);
	} else {
		printf("No\n");fflush(stdout);
	}
	printf("reg: 0x%x\n\n", reg);fflush(stdout);
}
