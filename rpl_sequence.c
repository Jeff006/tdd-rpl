#include "rpl.h"

//TODO: sequence counter stuff, section 7.2

int RPL_sequence_is_comparable(int a, int b) {
	//TODO
	return 0;
}

int RPL_sequence_is_greater(int a, int b) {
	//TODO
	return 0;
}

int RPL_sequence_is_lesser(int a, int b) {
	//TODO
	return 0;
}

//This algorithm (description). Wat.
//Must be (massively) optimizable
int RPL_sequence_counter_compare(int a, int b) {
	if ((a > 127) && (b <= 127)) {
		if ((256 + b - a) <= RPL_SEQUENCE_WINDOW) {
			return 1;   // B is greater than A
		} else {
			return -1;  // A is greater than B
		}
	} if ((a <= 127) && (b > 127)) {
		if ((256 + a - b) <= RPL_SEQUENCE_WINDOW) {
			return 1;   // B is greater than A
		} else {
			return -1;  // A is greater than B
		}
	}else {

	}


	return 0;
}

int RPL_sequence_counter_increment(int a) {
	if (a >= 128) {
		if (a == 255) {
			a = 0;
		} else {
			a ++;
		}
	} else {
		if (a == 127) {
			a = 0;
		} else {
			a ++;
		}
	}

	return a;
}

