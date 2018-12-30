/*
 * ps2.cpp - an interface library for ps2 devices.  Common devices are
 * mice, keyboard, barcode scanners etc.  See the examples for mouse and
 * keyboard interfacing.
 * limitations:
 *      we do not handle parity errors.
 *      The timing constants are hard coded from the spec. Data rate is
 *         not impressive.
 *      probably lots of room for optimization.
 */

#include "ps2.h"

int _ps2clk = CONFIG_HW_PS2CLK_GPIO;
int _ps2data = CONFIG_HW_PS2CLK_GPIO;
/*
 * the clock and data pins can be wired directly to the clk and data pins
 * of the PS2 connector.  No external parts are needed.
 */
void ps2_init()
{
	gohi(_ps2clk);
	gohi(_ps2data);
}

/*
 * according to some code I saw, these functions will
 * correctly set the clock and data pins for
 * various conditions.  It's done this way so you don't need
 * pullup resistors.
 */
void gohi(int pin)
{
	gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
	gpio_set_level((gpio_num_t)pin, 1);
}

void golo(int pin)
{
	gpio_set_direction((gpio_num_t)pin, GPIO_MODE_OUTPUT);
	gpio_set_level((gpio_num_t)pin, 0);
}

/* write a byte to the PS2 device */
void ps2_write(unsigned char data)
{
	unsigned char i;
	unsigned char parity = 1;
	
	gohi(_ps2data);
	gohi(_ps2clk);
	delayMicroseconds(10); //300
	golo(_ps2clk);
	delayMicroseconds(10); //300
	golo(_ps2data);
	delayMicroseconds(10);
	gohi(_ps2clk);	// start bit
	/* wait for device to take control of clock */
	while (gpio_get_level((gpio_num_t)_ps2clk) == 1)
		;	// this loop intentionally left blank
	// clear to send data
	for (i=0; i < 8; i++)
	{
		if (data & 0x01)
		{
			gohi(_ps2data);
		} else {
			golo(_ps2data);
		}
		// wait for clock
		while (gpio_get_level((gpio_num_t)_ps2clk) == 0)
			;
		while (gpio_get_level((gpio_num_t)_ps2clk) == 1)
			;
		parity = parity ^ (data & 0x01);
		data = data >> 1;
	}
	// parity bit
	if (parity)
	{
		gohi(_ps2data);
	} else {
		golo(_ps2data);
	}
	// clock cycle - like ack.
	while (gpio_get_level((gpio_num_t)_ps2clk) == 0)
		;
	while (gpio_get_level((gpio_num_t)_ps2clk) == 1)
		;
	// stop bit
	gohi(_ps2data);
	delayMicroseconds(50);
	while (gpio_get_level((gpio_num_t)_ps2clk) == 1)
		;
	// mode switch
	while ((gpio_get_level((gpio_num_t)_ps2clk) == 0) || (gpio_get_level((gpio_num_t)_ps2data) == 0))
		;
	// hold up incoming data
	golo(_ps2clk);
}


/*
 * read a byte of data from the ps2 device.  Ignores parity.
 */
unsigned char ps2_read(void)
{
	unsigned char data = 0x00;
	unsigned char i;
	unsigned char bit = 0x01;

	// start clock
	gohi(_ps2clk);
	gohi(_ps2data);
	delayMicroseconds(50);
	while (gpio_get_level((gpio_num_t)_ps2clk) == 1)
		;
	delayMicroseconds(5);	// not sure why.
	while (gpio_get_level((gpio_num_t)_ps2clk) == 0)
		;	// eat start bit
	for (i=0; i < 8; i++)
	{
		while (gpio_get_level((gpio_num_t)_ps2clk) == 1)
			;
		if (gpio_get_level((gpio_num_t)_ps2data) == 1)
		{
			data = data | bit;
		}
		while (gpio_get_level((gpio_num_t)_ps2clk) == 0)
			;
		bit = bit << 1;
	}
	// eat parity bit, ignore it.
	while (gpio_get_level((gpio_num_t)_ps2clk) == 1)
		;
	while (gpio_get_level((gpio_num_t)_ps2clk) == 0)
		;
	// eat stop bit
	while (gpio_get_level((gpio_num_t)_ps2clk) == 1)
		;
	while (gpio_get_level((gpio_num_t)_ps2clk) == 0)
		;
	golo(_ps2clk);	// hold incoming data

	return data;
}

