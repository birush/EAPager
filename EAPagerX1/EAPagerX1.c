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

// 16 bit Color Definitions ------
#define TFT_BLACK 0x0000
#define TFT_WHITE 0xFFFF
#define TFT_RED 0xF800
#define TFT_GREEN 0x07E0
#define TFT_BLUE 0x001F
#define TFT_ORANGE 0xFBE0
#define TFT_HS_BACKGROUND_COLOR 0x33FF
#define TFT_HS_BUTTON_COLOR 0x76EA
// -------------------------------

// Touch Sensing Calibration ------------------
#define ADC_X_OFFSET 510
#define X_COORD_MAX 400
#define ADC_X_MAX_MAG 3300
//#define ADC_X_MAX_MAG (4095-ADC_X_OFFSET)
#define ADC_Y_OFFSET 1100
#define Y_COORD_MAX 240
#define ADC_Y_MAX_MAG 3000
//#define ADC_Y_MAX_MAG (4095-ADC_Y_OFFSET)
// --------------------------------------------

// Timing Constants ---------------------------
#define VB_ONE 31250
#define VB_THREE_FOURTHS 23438
#define VB_HALF 15625
#define VB_THREE_EIGHTHS 11719
#define VB_FOURTH 7813
#define VB_EIGHTH 3906
#define VB_SIXTEENTH 1953
// --------------------------------------------


#define XAXIS ((char)0)
#define YAXIS ((char)1)
#define F_CPU 32000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
//#include <C:/Program Files (x86)/Atmel/Atmel Toolchain/AVR8 GCC/Native/3.4.1056/avr8-gnu-toolchain/avr/include/avr/pgmspace.h>
#include <avr/interrupt.h>
#include "EAimages.h"
#include "MazeGame.h"
#include "EAPagerX1.h"

volatile unsigned int adcValue = 0;
volatile unsigned int xTouch=0, yTouch=0;
volatile unsigned long xTouchCoord, yTouchCoord;
volatile unsigned char currentProgram = MAIN_MENU_ID;
volatile unsigned char changingProgram = 0;
volatile unsigned char vBCyclesLeft = 3;
unsigned int debug, debug1;

/*
const unsigned int ADC_X_OFFSET = 3250;
const unsigned int X_COORD_MAX = 400;
unsigned int ADC_X_MAX_MAG = 4096-ADC_X_OFFSET;
const unsigned int ADC_Y_OFFSET = 1000;
const unsigned int Y_COORD_MAX = 240;
unsigned int ADC_Y_MAX_MAG = 1460;
*/

const unsigned char LUT_RED_INC = 8;
const unsigned char LUT_GREEN_INC = 4;
const unsigned char LUT_BLUE_INC = 8;


int main(void)
{
	clock_init();
	//playMazeGame();
	PORTB_DIRSET = 0x60;	// Enable LED ports
	//PORTB_OUTTGL = 0x40;
	
	tft_init();
	
	touchInit();
	vibrate_init();
	//PORTB_OUTTGL = 0x40;
	
/*	tft_print_image(NUM0_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM1_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM2_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM3_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM4_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM5_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM6_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM7_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM8_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM9_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	_delay_ms(500);
	tft_print_image(NUM1_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX, PIL_STARTY);
	tft_print_image(NUM5_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, PIL_STARTX+PIL_SPACING, PIL_STARTY);
*/
    while(1)
    {
		//PORTB_OUTTGL = 0x20;
		//  Main Menu /////////////////////////////////////////////////////////////////////////
		if (currentProgram == MAIN_MENU_ID) {
			// Program initialization
			tft_print_image(HS_BACKGROUND_IMAGE_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, 0, 0);
			tft_print_image(HS_MAZES_BUTTON_IMAGE_ID, TFT_HS_BUTTON_COLOR, TFT_BLACK, HS_MAZES_BUTTON_STARTX, HS_MAZES_BUTTON_STARTY);
			tft_print_image(HS_SNAKE_BUTTON_IMAGE_ID, TFT_HS_BUTTON_COLOR, TFT_BLACK, HS_SNAKE_BUTTON_STARTX, HS_SNAKE_BUTTON_STARTY);
			changingProgram = 0;
			touchSenseReset();
			
			while (!changingProgram) {
				debug1 = 5;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////
		
		// Maze Game ///////////////////////////////////////////////////////////////////////////////
		else if (currentProgram == MAZE_GAME_ID) {
			// Program initialization
			tft_print_image(MAZE0_IMAGE_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, 0, 0);
			tft_print_image(MAIN_MENU_BUTTON_ID, TFT_HS_BUTTON_COLOR, TFT_BLACK, MAIN_MENU_BUTTON_STARTX, MAIN_MENU_BUTTON_STARTY);
			changingProgram = 0;
			touchSenseReset();

//			vibrate(VB_HALF);			
			vibrate_pulsed_start(VB_ONE, VB_EIGHTH, 5);
			
			while (!changingProgram) {
				debug1 = 3;
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////

    }

}

void vibrate_pulsed_start(unsigned int onTime, unsigned int offTime, unsigned char numCycles) {
	vBCyclesLeft = numCycles;
	TCF0_CTRLB = 0x60;		// Enable CCB and CCC only
	TCF0_INTCTRLB = 0x28;	// CCB, CCC interrupts enabled at medium priority
	unsigned int period = onTime + offTime;
	TCF0_PER = period;
	TCF0_CCB = onTime;			// Set on time
	TCF0_CCC = period;			// Set off time
	PORTB_OUTSET = 0x01;	// Turn on vibration motor
	TCF0_CTRLA = 0x07;	// Start vibration timer at 31.25 kHz
}

void vibrate_pulsed_stop() {
	PORTB_OUTCLR = 0x01;	// Turn off vibration motor
	TCF0_CTRLA = 0x00;		// Stop timer
	TCF0_CTRLFSET = 0x08;	//Restart timer
}

ISR(TCF0_CCB_vect) {
	PORTB_OUTCLR = 0x01;	// Turn off vibration motor
	if (--vBCyclesLeft == 0) {
		TCF0_CTRLA = 0x00;		// Stop timer
		TCF0_CTRLFSET = 0x08;	//Restart timer
	}
}

ISR(TCF0_CCC_vect) {
	PORTB_OUTSET = 0x01;	// Turn on vibration motor
}

void vibrate(unsigned int time32uS) {
	TCF0_CTRLB = 0x10;		// Enable CCA Only
	TCF0_INTCTRLB = 0x02;	// CCA interrupt enabled at medium priority
	TCF0_PER = time32uS;	// Set Top
	TCF0_CCA = time32uS;	// Set CCA	// Interrupt will occur at (time32uS * 32) us
	PORTB_OUTSET = 0x01;	// Turn on vibration motor
	TCF0_CTRLA = 0x07;	// Start vibration timer at 31.25 kHz
}

ISR(TCF0_CCA_vect) {
	PORTB_OUTCLR = 0x01;	// Turn off vibration motor
	TCF0_CTRLA = 0x00;		// Stop timer
	TCF0_CTRLFSET = 0x08;	//Restart timer
}

void vibrate_init() {
	PORTB_DIRSET = 0x01;	// Enable vibration control port

	// Setup Timer0F for timing
	PMIC_CTRL |= 0x02;	// Enable Medium level interrupts in PMIC
	TCF0_CTRLB = 0x00;	// Normal counting mode
}

void tft_print_square(unsigned int xCoord, unsigned int yCoord, unsigned int color, unsigned char size) {
	unsigned int expandedXCoord = 2*xCoord;
	unsigned int expandedYCoord	= 2*yCoord;
	unsigned char endOffset = size-1;
	setWindow(expandedXCoord, expandedXCoord+endOffset, expandedYCoord, expandedYCoord+endOffset);
	tft_write_command(0x2C);	//Begin RAMWR
	unsigned int area = size*size;
	for (int i=0; i < area; i++) {
		tft_write_data16(color);
	}
	//tft_write_command(0x00);
}

void tft_print_image(unsigned char imageId, unsigned int backgroundColor, unsigned int foregroundColor, unsigned int xCoord, unsigned int yCoord) {

////// Get array dimensions ///////////////////////////////////////////////
	unsigned int width, height;
	
	if(imageId <= 9) {
		width=NUM_WIDTH;
		height=NUM_HEIGHT;	
	}
	else if((imageId >= MAZE0_IMAGE_ID && imageId <= MAZE1_IMAGE_ID) || imageId == HS_BACKGROUND_IMAGE_ID)
	{
		width=FULL_IMAGE_WIDTH;
		height=FULL_IMAGE_HEIGHT;
	}
	else if (imageId >= HS_SNAKE_BUTTON_IMAGE_ID && imageId <= HS_MAZES_BUTTON_IMAGE_ID) {
		width=HS_BUTTON_IMAGE_WIDTH;
		height=HS_BUTTON_IMAGE_HEIGHT;
	}
	
	else if (imageId == MAIN_MENU_BUTTON_ID) {
		width = MAIN_MENU_BUTTON_WIDTH;
		height = MAIN_MENU_BUTTON_HEIGHT;
	}
	
	else {
		width = 0;
		height = 0;
	}
//////////////////////////////////////////////////////////////////////////////
	
	unsigned int startX = 2*xCoord;
	unsigned int endX = startX+(2*width)-1;
	unsigned int startY = 2*yCoord;
	unsigned int endY = startY+(2*height)-1;
	volatile unsigned char currentPixel;
	setWindow(startX, endX, startY, endY);
	tft_write_command(0x2C);	//Begin RAMWR
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			currentPixel = getPixel(imageId,x,y);
			if (currentPixel) {
				tft_write_data16(backgroundColor);
				tft_write_data16(backgroundColor);
			}
			else {
				tft_write_data16(foregroundColor);
				tft_write_data16(foregroundColor);
			}
		}
		
		for (int x=0; x<width; x++) {
			currentPixel = getPixel(imageId,x,y);
			if (currentPixel) {
				tft_write_data16(backgroundColor);
				tft_write_data16(backgroundColor);
			}
			else {
				tft_write_data16(foregroundColor);
				tft_write_data16(foregroundColor);
			}
		}
	}
	tft_write_command(0x00);
}

void alternateColors() {
		   		setWindow(0, 799, 0, 479);
		   		tft_write_command(0x2C);	//Begin RAMWR
		   		for (int y=0; y < 800; y++) {
			   		for (int x=0; x<480; x++) {
//				   		tft_write_data16(0xFF00);
//				   		tft_write_data16(0x00FF);
//				   		tft_write_data16(0x0000);
//				   		tft_write_data16(0xFFFF);
//						tft_write_data16(0xFFFF);
						tft_write_data16(0xFFFF);
				   		//tft_write_data16(0x0000);
			   		}
			   		//colorChar++;
			   		//colorInt++;
		   		}
		   		tft_write_command(0x00);
		   		
		   		tft_write_command(0x2C);	//Begin RAMWR
		   		for (int y=0; y < 800; y++) {
			   		for (int x=0; x<480; x++) {
				   		//tft_write_data8(colorChar);
//				   		tft_write_data16(0x00FF);
//				   		tft_write_data16(0x0000);
				   		tft_write_data16(0xFF00);
				   		//tft_write_data16(0xFFFF);
				   		//tft_write_data16(0x0000);
						//tft_write_data16(0x0000);
						//tft_write_data16(0x0000);
			   		}
			   		//colorChar++;
			   		//colorInt++;
		   		}
		   		tft_write_command(0x00);
}

void measureTouchCoordinates() {
	int numSamples = 16;
	// Checking Y coordinate ---------------------
	PORTA_DIRSET = 0x14;	// Set XL and XR as outputs
	PORTA_DIRCLR = 0x0A;	// Set YU and YD as inputs
	PORTA_OUTSET = 0x10;	// Set XR high
	PORTA_OUTCLR = 0x04;	// Set XL low
	yTouch=0;
	for (int i=0; i<numSamples; i++) {
		adc_take_sample('x');
		while (!(ADCA_INTFLAGS & (1 << 0))) {	//Wait for ADC to finish sampling
			debug=0;
		}
		yTouch += ADCA_CH0_RES;	// Read ADC value
		ADCA_INTFLAGS = 0x01;	// Clear ready flag
		_delay_us(4);
	}
	yTouch /= numSamples;

	// ------------------------------------------
	// Checking X coordinate --------------------
	PORTA_DIRSET = 0x0A;	// Set YU and YD as outputs
	PORTA_DIRCLR = 0x14;	// set XL and XR as inputs
	PORTA_OUTSET = 0x02;	// Set YU high
	PORTA_OUTCLR = 0x08;	// Set YD low
	xTouch=0;
	for (int i=0; i<numSamples; i++) {
		adc_take_sample('y');
		while (!(ADCA_INTFLAGS & (1 << 0))) {	//Wait for ADC to finish sampling
			debug=0;
		}
		xTouch += ADCA_CH0_RES;	// Read ADC value
		ADCA_INTFLAGS = 0x01;	// Clear ready flag
		_delay_us(4);
	}
	xTouch /= numSamples;
	PORTB_OUTTGL = 0x40;	// Toggle Red LED
	// ------------------------------------------
	//sei();	// Enable Interrupts
	//touchSenseInit();	// Setup pins to sense a touch
}

void touchInit() {
	adc_enable();
	touchSenseInit();
}

void touchSenseReset() {
	PORTA_DIRCLR = 0x16;	// Set XL, XR, YU as inputs
	PORTA_DIRSET = 0x08;	// Set YD as output
	PORTA_OUTCLR = 0x08;	// Set YD low
	PORTA_PIN4CTRL |= 0x03;	// XR sense falling edge
	//PORTA_PIN2CTRL |= 0x03;	// XL sense low level
	PORTB_OUTSET = 0x80;	// Set F7 high, external pullup for XR
	_delay_us(100);
	
	// Enable interrupt on XR change (PORTA_INT0) ----
	PORTA_INTFLAGS = 0x01;	// Clear PORTA Int0 Int Flag
	sei();
	PORTA_INTCTRL = 0x03;	// Enable PORTA INT0 at high priority
	PORTA_INT0MASK = 0x10;	// PA4 (XR) will trigger PORTA INT0
}

void touchSenseInit() {	
	PORTB_DIRSET = 0x80;	// Set B7 as output
	PORTA_DIRCLR = 0x16;	// Set XL, XR, YU as inputs	
	PORTA_DIRSET = 0x08;	// Set YD as output
	PORTA_OUTCLR = 0x08;	// Set YD low
	PORTA_PIN4CTRL |= 0x02;	// XR sense falling edge
	//PORTA_PIN2CTRL |= 0x03;	// XL sense low level
	PORTB_OUTSET = 0x80;	// Set B7 high, external pullup for XR
	
	
	// Enable interrupt on XR change (PORTA_INT0) ----
	PORTA_INTFLAGS |= 0x01;	// Clear PORTA Int0 Int Flag
	sei();
	PMIC_CTRL |= 0x04;	// Enable High level interrupts in PMIC
	PORTA_INTCTRL = 0x03;	// Enable INT0 at high priority
	PORTA_INT0MASK = 0x10;	// PA4 (XR) will trigger PORTA INT0
	
	// Setup Timer1 for Debouncing
	TCC1_CTRLB = 0x00;	// Normal counting mode
	TCC1_INTCTRLB = 0x0C;	// CCB interupt enabled at high priority
	TCC1_PER = 7813;	// Top = 7813
	TCC1_CCB = 7813;	// CCB = 7813	// Interrupt will occur at .25 s
}

// When Touch detected
ISR(PORTA_INT0_vect) {
	PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
	PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
	cli();
	PORTB_OUTTGL = 0x40;	// Toggle Red LED
	TCC1_CTRLA = 0x07;	// Start debounce timer at 31.25 kHz
	PORTB_OUTSET = 0x20;	// LED1 ON
	PORTB_OUTCLR = 0x80;	// Disable ext pull up on XR
	_delay_ms(80);
	measureTouchCoordinates();
	xTouchCoord = (volatile unsigned long)(xTouch & 0xFFF8);	// Discard lower 3 bits
	xTouchCoord -= (volatile unsigned long)ADC_X_OFFSET;
	xTouchCoord *= (volatile unsigned long)X_COORD_MAX;
	xTouchCoord /= (volatile unsigned long)ADC_X_MAX_MAG;
	yTouchCoord = (volatile unsigned long)(yTouch & 0xFFF8);	// Discard lower 4 bits
	yTouchCoord -= (volatile unsigned long)ADC_Y_OFFSET;
	yTouchCoord *= (volatile unsigned long)Y_COORD_MAX;
	yTouchCoord /= (volatile unsigned long)ADC_Y_MAX_MAG;
	tft_print_square(xTouchCoord, yTouchCoord, 0xFFFF, 10);		// show where touch was
	
	switch (currentProgram) {
		case MAIN_MENU_ID:
			if ((xTouchCoord > HS_MAZES_BUTTON_STARTX) && (xTouchCoord < (HS_MAZES_BUTTON_STARTX+HS_BUTTON_IMAGE_WIDTH))) {
				if ((yTouchCoord > HS_MAZES_BUTTON_STARTY) && (yTouchCoord < (HS_SNAKE_BUTTON_STARTY+HS_BUTTON_IMAGE_HEIGHT))) {
					currentProgram = 1;
					changingProgram = 1;
				}
			}
			break;
		case MAZE_GAME_ID:
			if ((xTouchCoord > MAIN_MENU_BUTTON_STARTX) && (xTouchCoord < (MAIN_MENU_BUTTON_STARTX+MAIN_MENU_BUTTON_WIDTH))) {
				if ((yTouchCoord > MAIN_MENU_BUTTON_STARTY) && (yTouchCoord < (MAIN_MENU_BUTTON_STARTY+MAIN_MENU_BUTTON_HEIGHT))) {
					currentProgram = 0;
					changingProgram = 1;
				}
			}
			break;
	}
	debug = 3;
}

// After Debouncing period, detect touches again
ISR(TCC1_CCB_vect) {
	TCC1_CTRLA = 0x00;	// Stop timer
	TCC1_CTRLFSET = 0x08;	//Restart timer
	if (!changingProgram) {
		touchSenseReset();
	}
	PORTA_INTFLAGS = 0x01;	// Clear PORTA Int0 Int Flag
	PORTB_OUTCLR = 0x20;	// LED1 OFF
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
	ADCA_REFCTRL |= ADC_REFSEL_INTVCC_gc;	// Use INTVCC=2.06 V as reference
	ADCA_PRESCALER |= ADC_PRESCALER_DIV32_gc;	// Set ADC Clk to 1 MHz
	ADCA_CH0_CTRL |= ADC_CH_INPUTMODE0_bm;	// Set as Single ended positive input signal
	//ADCA_CTRLB |= ADC_FREERUN_bm;	// Set to Freerunning
}

void tft_write_command(char command) {
	PORTA_OUTCLR = TFTDC;	// Set DC low (command)
	//PORTA &= TFTCS_L;
	PORTE_OUT = command;	// Put command on bus
	PORTA_OUTCLR = TFTWR;
	PORTA_OUTSET = TFTWR;	// Lock in data
	//PORTA |= TFTCS_H;
	//debug=0;
}

void tft_write_data8(char data8) {
	PORTA_OUTSET = TFTDC;	// Set DC high (data)
	//PORTA &= TFTCS_L;
	PORTE_OUT = data8;
	PORTA_OUTCLR = TFTWR;
	PORTA_OUTSET = TFTWR;	// Lock in data
	//PORTA |= TFTCS_H;
	//debug=0;
}

void tft_write_data16(unsigned int data) {
	PORTA_OUTSET = TFTDC;	// Set DC high (data)
	//PORTA &= TFTCS_L;
	PORTE_OUT = (char)(data & 0x00FF); // write low byte to DB
	PORTF_OUT = (char)((data & 0xFF00) >> 8); // write high byte to DB
	PORTA_OUTCLR = TFTWR;
	PORTA_OUTSET = TFTWR;	// Lock in data
	//PORTA |= TFTCS_H;
	//debug=0;
}

void setWindow(unsigned int xStart, unsigned int xEnd, unsigned int yStart, unsigned int yEnd) {
	tft_write_command(0x2A);				//Set column address
	tft_write_data8((char)(xStart >>8));	//Set Column Start
	tft_write_data8((char)xStart);
	tft_write_data8((char)(xEnd >> 8));		//Set column end
	tft_write_data8((char)xEnd);
	
	tft_write_command(0x2B);				//Set Page (Row) Address
	tft_write_data8((char)(yStart >>8));	//Set Page (Row) Start
	tft_write_data8((char)yStart);
	tft_write_data8((char)(yEnd >> 8));		//Set Page (Row) end
	tft_write_data8((char)yEnd);
	
}

void tft_init() {
	PORTE_DIRSET = 0xFF;
	PORTF_DIRSET = 0xFF;	//Set 16 bit data bus pins as outputs
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
	
	PORTB_OUTTGL = 0x20;
	
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

///////////////// // Define LUT for 16 bit mode ///////////////////////	
	volatile unsigned char LUTValue = 0x00;
	tft_write_command(0x2D);		// Define LUT for 16 bit mode
	
	for (int i=0; i < 64; i++) {	// Define Red section
		tft_write_data8(LUTValue);
		LUTValue += LUT_RED_INC;
	}
	
	LUTValue = 0x00;				// Define Green Section
	for (int i=0; i < 64; i++) {	
		tft_write_data8(LUTValue);
		LUTValue += LUT_GREEN_INC;
	}
	
	LUTValue = 0x00;				// Define Green Section
	for (int i=0; i < 64; i++) {
		tft_write_data8(LUTValue);
		LUTValue += LUT_BLUE_INC;
	}
///////////////////////////////////////////////////////////////////	
	
	tft_write_command(0x3A);	//Interface pixel format
	tft_write_data8(0x55);		//2 bytes per pixel
	
	//tft_write_command(0x3A);	//COLMOD
	//tft_write_data8(0x77);
	
	tft_write_command(0x36);	//MADCTL
	tft_write_data8(0xA0);
	
	tft_write_command(0x11);	//SLPOUT
	_delay_ms(120);
	
	tft_write_command(0x29);	//DISPON
	_delay_ms(10);
}

void debug1Function() {
	debug1=1;
	debug1=0;
}

void clock_init() {
	//PORTD_DIRSET = 0x80;		// Set PORTD7 as output
	//PORTCFG_CLKEVOUT = 0x02;	// Output per clock to PortD pin 7
	OSC_CTRL |= OSC_RC32MEN_bm;	// Enable 32 MHz internal Osc
//	OSC_CTRL |= OSC_RC32KEN_bm;	// Enable 32.768 kHz internal Oscillator
	while (!(OSC_STATUS & OSC_RC32MRDY_bm)) {	// Wait till new clock ready
		debug=3;
	}
	CCP = 0xD8; // Enable writing to protected IO Reg
	CLK_CTRL = 0x01;	// Use 32 MHz internal Osc as system clk
/*	DFLLRC32M_CTRL = 0x01;	// enable DFLL
	DFLLRC32M_COMP1 = 0xFE;
	DFLLRC32M_COMP2 = 0xCD;	// Set Oscillator Output to 54 MHz
*/
}