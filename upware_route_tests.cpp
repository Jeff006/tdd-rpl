

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "rpl.h"


TEST_GROUP(upward_route_tests)
{

	void setup() {
		CHECK_EQUAL(0, 0);
	}

	void teardown() {

	}
};


//8.1 DIO Base Rules
TEST(upward_route_tests, dio_base_rules_8_1) {

	//TODO: Check required fields are unaltered

	//TODO: Check DODAGID is valid address;
}

//8.2.1.  Neighbors and Parents within a DODAG Version
TEST(upward_route_tests, neighbors_parents_8_2_1) {
	//Check parent set must be a subset of neighbor set

	//Check root has parent set size of zero

	//Check non-root parent set >= 1 (how does this work with undiscovered?)

	//Check preferred parent must be a member of parent set

	//Check rank is greater than all elements in parent set

	//Check unreachable nodes are not considered in candidate neighbor set (see [RFC4861] for Neighbor Unreachability Detection)

	//Check unreachable nodes are removed from the routing table
}

//8.2.2.1.  DODAG Version
TEST(upward_route_tests, dodag_version_8_2_2_1) {
	//1. Check all elements of a parent set MUST belong to the same DODAG version

	//2. Check node is a member of a DODAG version only if all parents are members of that DODAG version
	//   (or if the node is the root)

	//3. Check node can only send DIOs for DODAG versions of which it is a member

	//4. Check DODAG root can increment DODAG version, follows Section 7
	//   Check non DODAG root cannot increment DODAG version

	//5. Check non-root node can not advertise a DODAGVersionNumber less than (<) that it is aware of

	//6. Check non-root node can not be a member of a previous version after advertising a higher (>) version

	//Check node holds DODAG information for implementation defined time following loss of all parents

	//Check parent advertising a new DODAGVersionNumber cannot belong to the sub-DODAG of a node advertising an older DODAGVersionNumber

}

//8.2.2.2.  DODAG Roots
TEST(upward_route_tests, dodag_roots_8_2_2_2) {
	//TODO:
}