/*
	trackpad.h
	Written by Joe Rickerby
*/

#ifndef Trackpad_h
#define Trackpad_h

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

class Trackpad
{
	public:
		Trackpad(int clk, int data);
		~Trackpad();
		packet_t * getNewPacket();
	private:
		void sendComSeq(char arg, bool setMode = false);
		PS2 * mouse;
		packet_t packet;
};

#endif /* trackpad_h */