/*
 * ps2.h - a library to interface with ps2 devices. See comments in
 * ps2.cpp.
 * Written by Chris J. Kiick, January 2008.
 * Release into public domain.
 */

#ifndef ps2_h
#define ps2_h


#include "driver/gpio.h"
#include "delay.h"

void ps2_init();
void ps2_write(unsigned char data);
unsigned char ps2_read(void);
void golo(int pin);
void gohi(int pin);

#endif /* ps2_h */

