/*
	trackpad.h
	Written by Joe Rickerby
*/

#ifndef trackpad_h
#define trackpad_h

#include "ps2.h"
//#include "cppfix.h"
#include "delay.h"

typedef struct _packet_t {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;
	bool leftButtonDown;
	bool rightButtonDown;
} packet_t;

void trackpad_init();
packet_t * getNewPacket();
void sendComSeq(char arg, bool setMode);
packet_t packet;

#endif /* trackpad_h */