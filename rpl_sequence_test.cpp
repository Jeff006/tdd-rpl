
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "rpl.h"


TEST_GROUP(sequence_tests)
{

	void setup() {
		CHECK_EQUAL(0, 0);
	}

	void teardown() {

	}
};

//Sequence requirement 1
TEST(sequence_tests, sequence_init_test) {

	int a;

	a = RPL_sequence_init();
	CHECK_EQUAL(RPL_SEQUENCE_INITIAL, a);
}

//Sequence requirement 2

TEST(sequence_tests, sequence_increment_test) {
	int a, b;

	//Linear low increment
	a = 1;
	b = RPL_sequence_increment(a);
	CHECK_EQUAL(a + 1, b);

	//Wraparound low
	a = 255;
	b = RPL_sequence_increment(a);
	CHECK_EQUAL(0, b):

	//Linear high increment
	a = 140;
	b = RPL_sequence_increment(a);
	CHECK_EQUAL(a + 1, b);

	//Wraparound high
	a = 255;
	b = RPL_sequence_increment(a);
	CHECK_EQUAL(0, b):
}

//Sequence requirement 3
TEST(sequence_tests, sequence_compare_test_3) {
	int a, b;
	int res;

	//Check Comparison between high/low values [RFC6550 Page 64 Section 3 Example A]
	a = 240;
	b = 5;

	res = RPL_sequence_is_comparable(a, b);
	CHECK_EQUAL(1, res);

	res = RPL_sequence_is_greater(a, b);
	CHECK_EQUAL(1, res);

	res = RPL_sequence_is_lesser(a, b);
	CHECK_EQUAL(0, res);


	//Check Comparison between high/low values [RFC6550 Page 64 Section 3 Example B]
	a = 250;
	b = 5;

	res = RPL_sequence_is_comparable(a, b);
	CHECK_EQUAL(1, res);

	res = RPL_sequence_is_greater(a, b);
	CHECK_EQUAL(0, res);

	res = RPL_sequence_is_lesser(a, b);
	CHECK_EQUAL(1, res);
}

//Sequence requirement 4
TEST(sequence_tests, sequence_compare_test_4) {
	//No idea how to test this, see [RFC6550 Page 65] 
}