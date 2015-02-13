/*
 * EAPagerX1.c
 *
 * Created: 2/5/2015 3:01:55 PM
 *  Author: Ben
 */ 

// To simplify TFT pin toggling----
#define TFTRES ((char)0x80)
//#define TFTCS ((char)0x40)
#define TFTDC ((char)0x40)
#define TFTWR ((char)0x20)
// -------------------------------

#define XAXIS ((char)0)
#define YAXIS ((char)1)
#define F_CPU 32000000

#include <avr/io.h>
#include <util/delay.h>
#include <C:/Program Files (x86)/Atmel/Atmel Toolchain/AVR8 GCC/Native/3.4.1056/avr8-gnu-toolchain/avr/include/avr/pgmspace.h>
#include <avr/interrupt.h>
#include "EAimages.h"
#include "EAPagerX1.h"


volatile unsigned int adcValue = 0;
volatile unsigned int xTouch=0, yTouch=0;
unsigned int debug, debug1;

int main(void)
{
	
	touchInit();
    while(1)
    {
		measureTouchCoordinates();
		debug=0;
	   
    }
}

void measureTouchCoordinates() {

	PORTA_INTCTRL &= 0x00;	
	cli();	//Disable Interrupts
	// Checking X coordinate ---------------------
	PORTA_DIRSET = 0x14;	// Set XL and XR as outputs
	PORTA_DIRCLR = 0x0A;	// Set YU and YD as inputs
	PORTA_OUTSET = 0x10;	// Set XR high
	PORTA_OUTCLR = 0x04;	// Set XL low
	adc_take_sample('x');
	while (!(ADCA_INTFLAGS & (1 << 0))) {	//Wait for ADC to finish sampling
		debug=0;
	}
	xTouch = ADCA_CH0_RES;	// Read ADC value
	// ------------------------------------------
	// Checking Y coordinate --------------------
	PORTA_DIRSET = 0x0A;	// Set YU and YD as outputs
	PORTA_DIRCLR = 0x14;	// set XL and XR as inputs
	PORTA_OUTSET = 0x02;	// Set YU high
	PORTA_OUTCLR = 0x08;	// Set YD low
	adc_take_sample('y');
	while (!(ADCA_INTFLAGS & (1 << 0))) {	//Wait for ADC to finish sampling
		debug=0;
	}
	yTouch = ADCA_CH0_RES;	// Read ADC value
	// ------------------------------------------
	sei();	// Enable Interrupts
	touchSenseInit();	// Setup pins to sense a touch
}

void touchInit() {
	adc_enable();
	touchSenseInit();
}

void touchSenseInit() {
		PORTA_DIRCLR = 0x16;	// Set XL, XR, YU as inputs
		PORTA_DIRSET = 0x08;	// Set YD as output
		PORTA_OUTCLR = 0x08;	// Set YD low
		PORTA_PIN4CTRL |= 0x1B;	//Set XR with pullup resistor and sense low level
		
		// Enable interrupt on XR change (PORTA_INT0) ----
		sei();
		PMIC_CTRL |= 0x02;	// Enable Medium level interrupts in PMIC
		PORTA_INTCTRL |= 0x20;
		PORTA_INTCTRL &= 0xFE;	// Enable INT0 at medium priority
		PORTA_INT0MASK |= 0x10;	// PA4 will trigger INT0
		//--------------------------------------------
}

// When Touch detected
ISR(PORTA_INT0_vect) {
	measureTouchCoordinates();
}

void adc_take_sample(char axis) {
	if (axis == 'x') {	// Using YU to measure X coordinate
		ADCA_CH0_MUXCTRL = 0x0D;	// Use ADC1 pin (YU)
		ADCA_CTRLA |= ADC_CH0START_bm; // Start single conversion
	}
	else {	// Using XL to measure Y coordinate
		ADCA_CH0_MUXCTRL = 0x15;	// Use ADC2 pin (XL)
		ADCA_CTRLA |= ADC_CH0START_bm; // Start single conversion
	}
}

void adc_enable() {
	ADCA_CTRLA |= ADC_ENABLE_bm;  // enable ADCA
	ADCA_REFCTRL |= ADC_REFSEL_AREFA_gc;	// Use AREFA as reference
	ADCA_PRESCALER |= ADC_PRESCALER_DIV32_gc;	// Set ADC Clk to 1 MHz
	ADCA_CH0_CTRL |= ADC_CH_INPUTMODE0_bm;	// Set as Single ended positive input signal
}

void tft_write_command(char command) {
	PORTA_OUTCLR = TFTDC;	// Set DC low (command)
	//PORTA &= TFTCS_L;
	PORTB_OUT = command;	// Put command on bus
	PORTA_OUTCLR = TFTWR;
	PORTA_OUTSET = TFTWR;	// Lock in data
	//PORTA |= TFTCS_H;
	//debug=0;
}

void tft_write_data8(char data8) {
	PORTA_OUTSET = TFTDC;	// Set DC high (data)
	//PORTA &= TFTCS_L;
	PORTB_OUT = data8;
	PORTA_OUTCLR = TFTWR;
	PORTA_OUTSET = TFTWR;	// Lock in data
	//PORTA |= TFTCS_H;
	//debug=0;
}

void tft_write_data16(unsigned int data) {
	PORTA_OUTSET = TFTDC;	// Set DC high (data)
	//PORTA &= TFTCS_L;
	PORTB_OUT = (char)(data & 0x00FF); // write low byte to DB
	PORTC_OUT = (char)((data & 0xFF00) >> 8); // write high byte to DB
	PORTA_OUTCLR = TFTWR;
	PORTA_OUTSET = TFTWR;	// Lock in data
	//PORTA |= TFTCS_H;
	//debug=0;
}

void tft_init() {
	PORTB_DIRSET = 0xFF;
	PORTC_DIRSET = 0xFF;	//Set 16 bit data bus pins as outputs
	PORTA_DIRSET = 0xE0;	//Set highest 3 bits of PORTA as outputs (tft control pins)
	
	//PORTA &= TFTCS_L;
	//PORTA &= TFTWR_L;
	PORTA_OUTSET = TFTRES;
	_delay_ms(120);
	PORTA_OUTCLR= TFTRES;	//reset TFT
	_delay_ms(100);
	PORTA_OUTSET= TFTRES;
	_delay_ms(120);		//wait 10 ms (may need to change to > 120)
	//PORTA |= TFTCS_H;
	PORTA_OUTSET = TFTWR;
	
	tft_write_command(0xB9);	//SETEXTC
	tft_write_data8(0xFF);
	tft_write_data8(0x83);
	tft_write_data8(0x69);
	
	tft_write_command(0xB1);	//SETPOWER
	tft_write_data8(0x01);
	tft_write_data8(0x00);
	tft_write_data8(0x34);
	tft_write_data8(0x06);
	tft_write_data8(0x00);
	tft_write_data8(0x0F);
	tft_write_data8(0x0F);
	tft_write_data8(0x2A);
	tft_write_data8(0x32);
	tft_write_data8(0x3F);
	tft_write_data8(0x3F);
	tft_write_data8(0x07);
	tft_write_data8(0x23);
	tft_write_data8(0x01);
	tft_write_data8(0xE6);
	tft_write_data8(0xE6);
	tft_write_data8(0xE6);
	tft_write_data8(0xE6);
	tft_write_data8(0xE6);
	
	tft_write_command(0xB2); //SETDISP
	tft_write_data8(0x00);
	tft_write_data8(0x20);
	tft_write_data8(0x0A);
	tft_write_data8(0x0A);
	tft_write_data8(0x70);
	tft_write_data8(0x00);
	tft_write_data8(0xFF);
	tft_write_data8(0x00);
	tft_write_data8(0x00);
	tft_write_data8(0x00);
	tft_write_data8(0x00);
	tft_write_data8(0x03);
	tft_write_data8(0x03);
	tft_write_data8(0x00);
	tft_write_data8(0x01);
	
	tft_write_command(0xB4);	//SETCYC
	tft_write_data8(0x00);
	tft_write_data8(0x18);
	tft_write_data8(0x80);
	tft_write_data8(0x10);
	tft_write_data8(0x01);
	
	tft_write_command(0xB6);	//SETVCOM
	tft_write_data8(0x2C);
	tft_write_data8(0x2C);
	
	tft_write_command(0xD5);	//SETGIP
	tft_write_data8(0x00);
	tft_write_data8(0x05);
	tft_write_data8(0x03);
	tft_write_data8(0x00);
	tft_write_data8(0x01);
	tft_write_data8(0x09);
	tft_write_data8(0x10);
	tft_write_data8(0x80);
	tft_write_data8(0x37);
	tft_write_data8(0x37);
	tft_write_data8(0x20);
	tft_write_data8(0x31);
	tft_write_data8(0x46);
	tft_write_data8(0x8A);
	tft_write_data8(0x57);
	tft_write_data8(0x9B);
	tft_write_data8(0x20);
	tft_write_data8(0x31);
	tft_write_data8(0x46);
	tft_write_data8(0x8A);
	tft_write_data8(0x57);
	tft_write_data8(0x9B);
	tft_write_data8(0x07);
	tft_write_data8(0x0F);
	tft_write_data8(0x02);
	tft_write_data8(0x00);
	
	tft_write_command(0xE0);		//GAMMA
	tft_write_data8(0x00);
	tft_write_data8(0x08);
	tft_write_data8(0x0D);
	tft_write_data8(0x2D);
	tft_write_data8(0x34);
	tft_write_data8(0x3F);
	tft_write_data8(0x19);
	tft_write_data8(0x38);
	tft_write_data8(0x09);
	tft_write_data8(0x0E);
	tft_write_data8(0x0E);
	tft_write_data8(0x12);
	tft_write_data8(0x14);
	tft_write_data8(0x12);
	tft_write_data8(0x14);
	tft_write_data8(0x13);
	tft_write_data8(0x19);
	tft_write_data8(0x00);
	tft_write_data8(0x08);
	tft_write_data8(0x0D);
	tft_write_data8(0x2D);
	tft_write_data8(0x34);
	tft_write_data8(0x3F);
	tft_write_data8(0x19);
	tft_write_data8(0x38);
	tft_write_data8(0x09);
	tft_write_data8(0x0E);
	tft_write_data8(0x0E);
	tft_write_data8(0x12);
	tft_write_data8(0x14);
	tft_write_data8(0x12);
	tft_write_data8(0x14);
	tft_write_data8(0x13);
	tft_write_data8(0x19);
	
	tft_write_command(0x3A);	//Interface pixel format
	tft_write_data8(0x77);		//3 bytes per pixel
	
	//tft_write_command(0x3A);	//COLMOD
	//tft_write_data8(0x77);
	
	tft_write_command(0x36);	//MADCTL
	tft_write_data8(0x80);
	
	tft_write_command(0x11);	//SLPOUT
	_delay_ms(120);
	
	tft_write_command(0x29);	//DISPON
	_delay_ms(10);
}

void tft_print_image(unsigned char *imageArray) {
	
}