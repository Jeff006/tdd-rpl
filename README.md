#Test Driven RPL Implementation

A project to evaluate the feasibility of hand compiling the RPL RFC (https://tools.ietf.org/html/rfc6550) into tests, to then allow development of RPL implementations against this. Because it seems silly that a) RFCs do not require example (and OSS) implementations and b) that there is not solid way of testing compliance with a particular RFC or standard.

Status:
 - Message Structures and Definitions - complete
 - Implemented sequence counter tests
 - Started working on upward routing tests, not yet sure how to implement.

Seems like building these tests will impose interface requirements on the implementation, also not sure if this is a major problem.


