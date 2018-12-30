/*
  trackpad.cpp - Library for interfacing with synaptic trackpads.
  Created by Joe Rickerby, July 2008.
  Released into the public domain.
*/
#include "trackpad.h"

void trackpad_init()
{
	ps2_init();
	ps2_write(0xff);	// reset
	ps2_read();	 // ack byte
	ps2_read();	 // blank 
	ps2_read();	 // blank 
	ps2_write(0xf0);	// remote mode
	ps2_read();	 // ack
	
	//set mode byte- absolute mode, wmode
	sendComSeq(0xC1, 1); //C0 absolute, high, no w. C1 absolute, high, w/ w.
	delayMicroseconds(100);
}

//Details are available in the synaptic interfacing guide- this is a bit of a nasty one.
//Synaptic special commands are hidden in Set Resolution commands, so that the data will make
//it through the BIOS on a PC unscathed.
void sendComSeq(char arg, bool setMode)
{
	
	ps2_write(0xe8);
	ps2_write((arg & 0xc0)>>6);
	ps2_read();	 // ack byte
	ps2_write(0xe8);
	ps2_write((arg & 0x30)>>4);
	ps2_read();	 // ack byte
	ps2_write(0xe8);
	ps2_write((arg & 0x0c)>>2);
	ps2_read();	 // ack byte
	ps2_write(0xe8);
	ps2_write(arg & 0x03);
	ps2_read();	 // ack byte

	if(setMode == 0) {
		ps2_write(0xe9);
		ps2_read();	 // ack byte
		/*
		//If you want to read a ComSeq, you can grab the data here. I'm not actually using
		//these ComSeqs for anything but setting mode bits, so I've left it out.
		d1=ps2_read();
		d2=ps2_read();
		d3=ps2_read();
		*/
	} else {
		ps2_write(0xf3);
		ps2_read();	 // ack byte
		ps2_write(0x14);
		ps2_read();	 // ack byte
	}

}

/*
	See the Synaptic interfacing guide for the derivation of all that bitwise nonsense
	down there.
*/
packet_t * getNewPacket()
{
	char p1;
	char p2;
	char p3;
	char p4;
	char p5;
	char p6;
	
	ps2_write(0xeb);	 // give me data!
	ps2_read();			// ignore ack
	p1 = ps2_read();
	p2 = ps2_read();
	p3 = ps2_read();
	p4 = ps2_read();
	p5 = ps2_read();
	p6 = ps2_read();

	unsigned int x,y;
	x = p5;
	x |= (p2 & 0x0f) << 8;
	x |= (p4 & 0x10) << 8;
	packet.x = (x > 1288 ? x - 1288 : 0) / 11;
	
	y = p6;
	y |= ((p2 & 0xf0) << 4);
	y |= ((p4 & 0x20) << 7);
	packet.y = (y > 1086 ? y - 1086 : 0) / 9.5;
	
	packet.z = p3;
	
	packet.w = 0;
	packet.w |= (p4 & 0x04) >> 2;
	packet.w |= (p1 & 0x04) >> 1;
	packet.w |= (p1 & 0x30) >> 2;
	
	packet.leftButtonDown = p1 & 0x01;
	packet.rightButtonDown = (p1 & 0x02) >> 1;

	return &packet;
}
