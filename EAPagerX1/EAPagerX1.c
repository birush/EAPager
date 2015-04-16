/*
 * EAPagerX1.c
 *
 * Created: 2/5/2015 3:01:55 PM
 *  Author: Ben
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "EAimages.h"
#include "EAPagerX1.h"
#include <util/delay.h>

// To simplify TFT pin toggling----
#define TFTRES ((char)0x80)
//#define TFTCS ((char)0x40)
#define TFTDC ((char)0x40)
#define TFTWR ((char)0x20)
// -------------------------------

// Touch Sensing Calibration ----------------------
// Blue Pager ///////////////////////////////
#ifdef BLUE1
#define ADC_X_OFFSET 510
#define X_COORD_MAX 400
#define ADC_X_MAX_MAG 3250
//#define ADC_X_MAX_MAG (4095-ADC_X_OFFSET)
#define ADC_Y_OFFSET 975
#define Y_COORD_MAX 240
#define ADC_Y_MAX_MAG 3200
//#define ADC_Y_MAX_MAG (4095-ADC_Y_OFFSET)
#endif
/////////////////////////////////////////////
// Orange Pager /////////////////////////////
#ifdef ORANGE2
#define ADC_X_OFFSET 500
#define X_COORD_MAX 400
#define ADC_X_MAX_MAG 3250
//#define ADC_X_MAX_MAG (4095-ADC_X_OFFSET)
#define ADC_Y_OFFSET 1000
#define Y_COORD_MAX 240
#define ADC_Y_MAX_MAG 3250
//#define ADC_Y_MAX_MAG (4095-ADC_Y_OFFSET)
#endif
/////////////////////////////////////////////
// -------------------------------------------------

#define XAXIS ((char)0)
#define YAXIS ((char)1)

volatile unsigned int adcValue = 0;
volatile unsigned int xTouch=0, yTouch=0;
volatile unsigned long xTouchCoord, yTouchCoord;
volatile unsigned char vBCyclesLeft = 3;
volatile unsigned int tempa=65000;


const unsigned char LUT_RED_INC = 8;
const unsigned char LUT_GREEN_INC = 4;
const unsigned char LUT_BLUE_INC = 8;


// Frankie --------------------------------------------
//GLOBALS NEEDED FOR SOUND
const unsigned long int clkFreq=32000000;
const unsigned int preScaler=1;
const unsigned int sampleNum = 50;
unsigned int noteLength=8;
unsigned int currentFreq=0;
unsigned int subCount2=0,subCount3=1;
unsigned long int subCount1=0,uberCount=0;
unsigned int currentSongStartIndex;
unsigned char currentSongLoop, songPlaying;
unsigned int currentSongSecondsLeft;
unsigned char silentNote;
unsigned int unitNumber;
unsigned char loopsLeft;





//unsigned long int musicArray[4]={0x0400 | 49,0x0800| 1,0x0400|88,0};
//unsigned long int musicArray[12]={0x431,0x231,0x832,0x22D,0x22E,0x22F,0x230,0x12F,0x130,0x12F,0x130,0};
unsigned long int musicArray[149]=
{
	/*TRILL*/0x1044,0x103C,0x103D,0x103C,0x103D,0x103C,0x103D,0x83C,0x83D,0x83C,0x83D,0,/*WT_INTRO*/0x63D,0x13D,
	0x13D,0x63D,0x13D,0x13D,0x23D,0x23A,0x236,0x23A,0x23D,0x23A,0x23D,0x236,0x23D,0x23A,
	0x236,0x23A,0x23D,0x23A,0x3D,0x236,0xE3D,0,/*WT_VERSE*/0x83D,0x100,0x83D,0x100,0x103D,0x100,
	0x83D,0x100,0x83D,0x100,0x103D,0x100,0x83D,0x100,0x83D,0x100,0x1042,0x100,0x1044,0x100,0x1046,
	0x100,0x83D,0x100,0x83D,0x100,0x103D,0x100,0x83D,0x100,0x83D,0x100,0x1042,0x100,0x846,0x100,
	0x846,0x100,0x1044,0x100,0x1041,0x100,0x103D,0x100,0x83D,0x100,0x83D,0x100,0x103D,0x100,0x83D,
	0x100,0x83D,0x100,0x103D,0x100,0x83D,0x100,0x83D,0x100,0x1042,0x100,0x1044,0x100,0x1046,0x100,
	0x842,0x100,0x846,0x100,0x283D,0x100,0x83B,0x100,0x846,0x100,0x844,0x100,0x1042,0x100,0x1046,
	0x100,0x1042,0x2000,0,/*ALERT*/0x844,0x845,0x846,0x847,0x848,0,/*ATTN*/0x2042,0x2046,0x2049,0,
	/*GAMEOVER*/0x103D,0x100,0x103C,0x100,0x103B,0x100,0x403A,0x100,0,/*SUCCESS*/0x203B,0x100,
	0x103B,0x100,0x103B,0x100,0x4042,0x100,0
};
unsigned int musicArrayCount=0;//Change datatype as required

unsigned int sinArray[50],id1=0,id3=0;
unsigned long int id2=0;//DEBUG
//const double freqArray[88]={0,27.5,29.135,32.703,34.648,36.708,38.891,41.204,43.654,46.250,49.000,51.913,55,58.270,61.735,65.406,69.296,73.416,77.782,82.407,87.307,92.499,97.999,103.826,110,116.541,123.471,130.813,138.591,146.832,155.563,164.814,174.614,184.997,195.998,207.652,220,233.082,246.942,261.626,277.183,293.665,311.127,329.628,349.228,369.994,391.995,415.305,440,466.164,493.883,523.251,554.365,587.330,622.254,659.255,698.456,739.989,783.991,830.609,880,932.328,987.767,1046.5,1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760,1864,1975.53,2093,2217.46,2349.32,2489.02,2637.02,2793.83,2959.96,3135.96,3322.44,3520,3729.31,3951.07,4186.01};
const double freqArray[89]=
{
	0,27.5,29.135,30.87,32.703,34.648,36.708,38.891,41.204,43.654,46.250,49.000,51.913,
	55,58.270,61.735,65.406,69.296,73.416,77.782,82.407,87.307,92.499,97.999,103.826,
	110,116.541,123.471,130.813,138.591,146.832,155.563,164.814,174.614,184.997,195.998,
	207.652,220,233.082,246.942,261.626,277.183,293.665,311.127,329.628,349.228,369.994,
	391.995,415.305,440,466.164,493.883,523.251,554.365,587.330,622.254,659.255,698.456,
	739.989,783.991,830.609,880,932.328,987.767,1046.5,1108.73,1174.66,1244.51,1318.51,
	1396.91,1479.98,1567.98,1661.22,1760,1864,1975.53,2093,2217.46,2349.32,2489.02,2637.02,
	2793.83,2959.96,3135.96,3322.44,3520,3729.31,3951.07,4186.01
};
// --------------------------------------------------------------------------


int main(void)
{
	clock_init();
	//PORTB_DIRSET = 0x60;	// Enable LED ports
	//PORTB_OUTTGL = 0x40;
	
	#ifdef BLUE1
	unitNumber = 1;
	#endif

	#ifdef ORANGE2
	unitNumber = 2;
	#endif
	
	tft_init();
	touchInit();
	vibrate_init();
	SPID_init();
	TIMER_CONFIG();
	SPIC_init();
	COM_INIT();
	COM_RX_MODE();
	
	currentProgram = MAIN_MENU_ID;
	changingProgram = 0;
	resumingProgram = 0;
	pagingUser = 0;
	outOfRange = 0;
	tableReady = 0;
	placeInLine = (volatile unsigned char*)malloc(sizeof(volatile unsigned char*));
	*placeInLine = 255;
	PILChanged = 0;
	touchDebounceDelay = 80;
	currentMaze = 0;
	currentSongLoop = 0;
	songPlaying = 0;
	soundOn = 0;
	silentNote = 0;
	loopsLeft = 0;
	//unitNumber = 1;
	calculateDigits(placeInLine, PILDigits); // DEBUGGING
	makeSineWave();
	//playSong(WTVERSE_SONG_ID, 1, 30);
	//vibrate_pulsed_start(VB_ONE, VB_EIGHTH, 2);
	
	// Initialize OOR Timer ///////////////////////
	timeSincePing = 0;
	PMIC_CTRL |= 0x01;		// Enable Low level interrupts in PMIC
	TCD0_CTRLB = 0x10;			// Normal counting mode, Enable CCA Only
	//#ifdef HAVE_BASE
	//TCD0_INTCTRLB = 0x01;		// CCA interrupt enabled at low priority
	//#endif
	TCD0_PER = VB_TWO;			// Set Top
	TCD0_CCA = VB_TWO;			// Set CCA	
	TCD0_CTRLA = 0x07;			// Start OOR timer
	//////////////////////////////////////////

    while(1)
    {
		//  Main Menu /////////////////////////////////////////////////////////////////////////
		if (currentProgram == MAIN_MENU_ID) {
			// Program initialization
			tft_print_image(HS_BACKGROUND_IMAGE_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, 0, 0);
			tft_print_image(HS_MAZES_BUTTON_IMAGE_ID, TFT_HS_BUTTON_COLOR, TFT_BLACK, HS_MAZES_BUTTON_STARTX, HS_MAZES_BUTTON_STARTY);
			tft_print_image(HS_SNAKE_BUTTON_IMAGE_ID, TFT_HS_BUTTON_COLOR, TFT_BLACK, HS_SNAKE_BUTTON_STARTX, HS_SNAKE_BUTTON_STARTY);
			printDigits(TFT_HS_BACKGROUND_COLOR, PILDigits, PIL_STARTX, PIL_STARTY);
			changingProgram = 0;
			touchSenseReset();
			
			while (!changingProgram) {
				if (PILChanged) {
					printDigits(TFT_HS_BACKGROUND_COLOR, PILDigits, PIL_STARTX, PIL_STARTY);
					PILChanged = 0;
				}
				
				if (tableReady) {
					// Disable Touch Sensing
					PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
					PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
					
					tableReadyMessage();
				}
				
				if (pagingUser) {
					// Disable Touch Sensing
					PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
					PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
					
					pageUser();
				}
				
				if (outOfRange) {
					// Disable Touch Sensing
					PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
					PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
					
					timeSincePing = 0;
					showOORMessage();
				}
				
				if (resumingProgram) {
					currentProgram = MAIN_MENU_ID;
					
					// Reprint Main Menu Screen //////////////////////////////
					tft_print_image(HS_BACKGROUND_IMAGE_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, 0, 0);
					tft_print_image(HS_MAZES_BUTTON_IMAGE_ID, TFT_HS_BUTTON_COLOR, TFT_BLACK, HS_MAZES_BUTTON_STARTX, HS_MAZES_BUTTON_STARTY);
					tft_print_image(HS_SNAKE_BUTTON_IMAGE_ID, TFT_HS_BUTTON_COLOR, TFT_BLACK, HS_SNAKE_BUTTON_STARTX, HS_SNAKE_BUTTON_STARTY);
					printDigits(TFT_HS_BACKGROUND_COLOR, PILDigits, PIL_STARTX, PIL_STARTY);
					resumingProgram = 0;
					touchSenseReset();
					//////////////////////////////////////////////////////////
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////
		
		// Maze Game ///////////////////////////////////////////////////////////////////////////////
		else if (currentProgram == MAZE_GAME_ID) {
			playMazeGame();
		}
		///////////////////////////////////////////////////////////////////////////////////////

		// Snake Game ///////////////////////////////////////////////////////////////////////////////
		else if (currentProgram == SNAKE_GAME_ID) {
			playSnakeGame();
		}
		/////////////////////////////////////////////////////////////////////////////////////////////
/*
		// Table Ready ///////////////////////////////////////////////////////////////////////////////
		else if (currentProgram == TABLE_READY_PG_ID) {
			tft_print_blank_background(TFT_HS_BACKGROUND_COLOR);
			tft_print_image(TABLE_READY_IMAGE_ID, GAME_BACKGROUND_COLOR, TFT_BLACK, TABLE_READY_STARTX, TABLE_READY_STARTY);
			tft_print_image(OK_BUTTON_BC_ID, TFT_GREEN, TFT_BLACK, OK_BUTTON_BC_STARTX, OK_BUTTON_BC_STARTY);
			
			vibrate_pulsed_start(VB_FOURTH, VB_EIGHTH, 20);
			playSong(WTVERSE_SONG_ID,1, 60);
			changingProgram = 0;
			touchSenseReset();
			
			while (!changingProgram) {
				tft_print_image(TABLE_READY_IMAGE_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, TABLE_READY_STARTX, TABLE_READY_STARTY);
				//_delay_ms(100);
				tft_print_image(TABLE_READY_IMAGE_ID, GAME_BACKGROUND_COLOR, TFT_BLACK, TABLE_READY_STARTX, TABLE_READY_STARTY);
			}
			
		}
		////////////////////////////////////////////////////////////////////////////////////////////
*/
    }

}

void tableReadyMessage() {
	currentProgram = TABLE_READY_PG_ID;
	tft_print_blank_background(TFT_HS_BACKGROUND_COLOR);
	tft_print_image(TABLE_READY_IMAGE_ID, GAME_BACKGROUND_COLOR, TFT_BLACK, TABLE_READY_STARTX, TABLE_READY_STARTY);
	tft_print_image(OK_BUTTON_BC_ID, TFT_GREEN, TFT_BLACK, OK_BUTTON_BC_STARTX, OK_BUTTON_BC_STARTY);
	
	vibrate_pulsed_start(VB_FOURTH, VB_EIGHTH, 20);
	playSong(WTVERSE_SONG_ID,1, 10);
	touchSenseReset();
	
	while (!resumingProgram) {
		tft_print_image(TABLE_READY_IMAGE_ID, TFT_HS_BACKGROUND_COLOR, TFT_BLACK, TABLE_READY_STARTX, TABLE_READY_STARTY);
		_delay_ms(100);
		tft_print_image(TABLE_READY_IMAGE_ID, GAME_FOREGROUND_COLOR, TFT_BLACK, TABLE_READY_STARTX, TABLE_READY_STARTY);
	}
	
	#ifdef HAVE_BASE
	TCD0_INTCTRLB = 0x00;		// CCA interrupt disabled
	//TCD0_CTRLA = 0x00;			// Stop OOR timer
	//TCD0_CTRLFSET = 0x08;		// restart OOR timer
	timeSincePing = 0;
	#endif
}

void pageUser() {
	currentProgram = PAGING_USER_PG_ID;
	tft_print_image(PAGING_USER_IMAGE_ID, GAME_BACKGROUND_COLOR, TFT_BLACK, PAGING_USER_STARTX, PAGING_USER_STARTY);
	tft_print_image(OK_BUTTON_BC_ID, TFT_GREEN, TFT_BLACK, OK_BUTTON_BC_STARTX, OK_BUTTON_BC_STARTY);
	
	vibrate_pulsed_start(VB_THREE_FOURTHS, VB_EIGHTH, 20);
	playSong(ATTN_SONG_ID,1, 20);
	touchSenseReset();
	
	while (!resumingProgram) {

	}
}

void showOORMessage() {
	currentProgram = OOR_PG_ID;
	tft_print_image(OOR_IMAGE_ID, GAME_BACKGROUND_COLOR, TFT_BLACK, OOR_STARTX, OOR_STARTY);
	tft_print_image(OK_BUTTON_BC_ID, TFT_GREEN, TFT_BLACK, OK_BUTTON_BC_STARTX, OK_BUTTON_BC_STARTY);
	
	vibrate_pulsed_start(VB_THREE_FOURTHS, VB_EIGHTH, 20);
	playSong(ALERT_SONG_ID,1, 20);
	touchSenseReset();
	
	while (!resumingProgram) {

	}
}

void playSong(unsigned int songId, unsigned char loopOn, unsigned char numLoops) {
	soundOn = 1;
	musicArrayCount = songId;
	currentSongStartIndex = songId;
	currentSongLoop = loopOn;

	if (loopOn) {
		loopsLeft = numLoops;
/*
		currentSongSecondsLeft = durationS;
		PMIC_CTRL |= 0x01;		// Enable Low level interrupts in PMIC
		TCD0_CTRLB = 0x10;			// Normal counting mode, Enable CCA Only
		TCD0_INTCTRLB = 0x01;		// CCA interrupt enabled at low priority
		tcd0_prevPER = TCD0_PER;	// Save old per
		tcd0_prevCTRLA = TCD0_CTRLA;	// save old on state and speed
		TCD0_PER = VB_ONE;			// Set Top
		TCD0_CCA = VB_ONE;			// Set CCA	// Interrupt will occur at (duration32us * 32) us
		TCD0_CTRLA = 0x07;			// Start song duration timer
*/
	}
	soundPlay();
}

ISR(TCD0_CCA_vect) {
	timeSincePing +=2;
	if (timeSincePing > 20) {
		outOfRange = 1;
	}

/*
	if (--currentSongSecondsLeft == 0) {
		soundStop();
		TCD0_PER = tcd0_prevPER;
		TCD0_CTRLA = tcd0_prevCTRLA;
		TCE0_CTRLB = 0x00;		// Disable CCA
		TCE0_INTCTRLB = 0x00;	// Disable TCE0 interrupts
	}
*/	
}

void printDigits(unsigned int backgroundColor, volatile unsigned char digits[4], unsigned int digitStartX, unsigned int digitStartY) {
	if (digits[3] == 255) {
		return;
	}
	
	if (digits[0] == 0) {
		if (digits[1] != 0) {
			tft_print_image(digits[1], backgroundColor, TFT_BLACK, digitStartX, digitStartY);
			tft_print_image(digits[2], backgroundColor, TFT_BLACK, digitStartX+DIGIT_OFFSET1, digitStartY);
			tft_print_square(digitStartX+DIGIT_OFFSET2, digitStartY, backgroundColor, 16);
		}
		else {
			tft_print_image(digits[2], backgroundColor, TFT_BLACK, digitStartX, digitStartY);
			tft_print_square(digitStartX+DIGIT_OFFSET1, digitStartY, backgroundColor, 16);
			tft_print_square(digitStartX+DIGIT_OFFSET2, digitStartY, backgroundColor, 16);
		}
	}
	else {
		tft_print_image(digits[0], backgroundColor, TFT_BLACK, digitStartX, digitStartY);
		tft_print_image(digits[1], backgroundColor, TFT_BLACK, digitStartX+DIGIT_OFFSET1, digitStartY);
		tft_print_image(digits[2], backgroundColor, TFT_BLACK, digitStartX+DIGIT_OFFSET2, digitStartY);
	}
}

void calculateDigits(volatile unsigned char* number, volatile unsigned char digits[4])
{
	if (*number == 255) {
		digits[3] = 255;
	}
	else {
		digits[3] = 0;
	}
	
	if (*number < 100) {
		digits[0] = 0;
		debug = *number/10;
		digits[1] = debug;
		debug = *number % 10;
		digits[2] = debug;
	}
	
	else {
		digits[0] = *number/100;
		digits[1] = *number % 100;
		digits[2] = digits[1] % 10;
		digits[1] = digits[1]/10;
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
	unsigned char endOffset = (2*size)-1;
	setWindow(expandedXCoord, expandedXCoord+endOffset, expandedYCoord, expandedYCoord+endOffset);
	tft_write_command(0x2C);	//Begin RAMWR
	unsigned int area = 4*size*size;
	for (int i=0; i < area; i++) {
		tft_write_data16(color);
	}
	//tft_write_command(0x00);
}

void tft_print_blank_background(unsigned int color) {	
	setWindow(0, SCREEN_WIDTH_END, 0, SCREEN_HEIGHT_END);
	tft_write_command(0x2C);	//Begin RAMWR
	for (int i=0; i < SCREEN_HEIGHT; i++) {
		for (int j=0; j < SCREEN_WIDTH; j++) {
			tft_write_data16(color);
		}
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
/*	else if (imageId == HS_BACKGROUND_IMAGE_ID || imageId == SG_BACKGROUND_IMAGE_ID) {
		width=FULL_IMAGE_WIDTH;
		height=FULL_IMAGE_HEIGHT;
	}
*/	
	else if (imageId >= MAZE0_PATH_ID && imageId < MAZE0_IMAGE_ID) {
		width = MAZEPATH_WIDTH;
		height = MAZEPATH_HEIGHT;
	}
	else if(imageId >= MAZE0_IMAGE_ID)
	{
		width = FULL_IMAGE_WIDTH;
		height = FULL_IMAGE_HEIGHT;
	}
	else if (imageId == HS_SNAKE_BUTTON_IMAGE_ID || imageId == HS_MAZES_BUTTON_IMAGE_ID) {
		width=HS_BUTTON_IMAGE_WIDTH;
		height=HS_BUTTON_IMAGE_HEIGHT;
	}
	
	else if (imageId == MAIN_MENU_BUTTON_ID) {
		width = MAIN_MENU_BUTTON_WIDTH;
		height = MAIN_MENU_BUTTON_HEIGHT;
	}
	
	else if (imageId == SNAKE_GAME_OVER_ID || imageId == MAZE_SOLVED_IMAGE_ID) {
		width = SNAKE_GAME_OVER_WIDTH;
		height = SNAKE_GAME_OVER_HEIGHT;
	}
	
	else if (imageId == OK_BUTTON_BC_ID) {
		width = OK_BUTTON_BC_WIDTH;
		height = OK_BUTTON_BC_HEIGHT;
	}
	
	else if (imageId == TABLE_READY_IMAGE_ID) {
		width = TABLE_READY_WIDTH;
		height = TABLE_READY_HEIGHT;
	}

/*	else if (imageId == PAGING_USER_IMAGE_ID) {
		width = PAGING_USER_WIDTH;
		height = PAGING_USER_HEIGHT;
	}
*/	
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
	PORTB_OUTSET = 0x80;	// Set B7 high, external pullup for XR
	_delay_us(400);
	
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
	_delay_us(400);
	
	
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
	//PORTB_OUTTGL = 0x40;	// Toggle Red LED
	//PORTB_OUTSET = 0x20;	// LED1 ON
	PORTB_OUTCLR = 0x80;	// Disable ext pull up on XR

	// Debouncing (shorter if in maze game)
//	if (currentProgram == MAZE_GAME_ID) {
//		_delay_ms(80);
//	}
//	else {
		TCC1_CTRLA = 0x07;	// Start debounce timer at 31.25 kHz
		_delay_ms(80);
//	}

	measureTouchCoordinates();
	xTouchCoord = (volatile unsigned long)(xTouch & 0xFFF8);	// Discard lower 3 bits
	xTouchCoord -= (volatile unsigned long)ADC_X_OFFSET;
	xTouchCoord *= (volatile unsigned long)X_COORD_MAX;
	xTouchCoord /= (volatile unsigned long)ADC_X_MAX_MAG;
	yTouchCoord = (volatile unsigned long)(yTouch & 0xFFF8);	// Discard lower 4 bits
	yTouchCoord -= (volatile unsigned long)ADC_Y_OFFSET;
	yTouchCoord *= (volatile unsigned long)Y_COORD_MAX;
	yTouchCoord /= (volatile unsigned long)ADC_Y_MAX_MAG;

	//	Testing tableReady, pagingUser, outOfRange	
	//	if (xTouchCoord < 50) {
	//		if (yTouchCoord < 20) {
	//			tableReady = 1;
	//			return;
	//		}
	//	}

	
	switch (currentProgram) {
		case MAIN_MENU_ID:
			tft_print_square(xTouchCoord, yTouchCoord, 0xFFFF, 5);		// show where touch was
			if ((xTouchCoord > HS_MAZES_BUTTON_STARTX) && (xTouchCoord < (HS_MAZES_BUTTON_STARTX+HS_BUTTON_IMAGE_WIDTH))) {
				if ((yTouchCoord > HS_MAZES_BUTTON_STARTY) && (yTouchCoord < (HS_SNAKE_BUTTON_STARTY+HS_BUTTON_IMAGE_HEIGHT))) {
					currentProgram = MAZE_GAME_ID;
					changingProgram = 1;
				}
			}
			else if ((xTouchCoord > HS_SNAKE_BUTTON_STARTX) && (xTouchCoord < (HS_SNAKE_BUTTON_STARTX+HS_BUTTON_IMAGE_WIDTH))) {
				if ((yTouchCoord > HS_SNAKE_BUTTON_STARTY) && (yTouchCoord < (HS_SNAKE_BUTTON_STARTY+HS_BUTTON_IMAGE_HEIGHT))) {
					currentProgram = SNAKE_GAME_ID;
					changingProgram = 1;
				}
			}
			break;
		case MAZE_GAME_ID:
			if (inMazeIntro) {
				if ((xTouchCoord > OK_BUTTON_RC_STARTX) && (xTouchCoord < (OK_BUTTON_RC_STARTX+OK_BUTTON_RC_WIDTH))) {
					if ((yTouchCoord > OK_BUTTON_RC_STARTY) && (yTouchCoord < (OK_BUTTON_RC_STARTY+OK_BUTTON_RC_HEIGHT))) {
						inMazeIntro = 0;
						return;
					}
				}
			}
			if ((xTouchCoord > MAIN_MENU_BUTTON_STARTX) && (xTouchCoord < (MAIN_MENU_BUTTON_STARTX+MAIN_MENU_BUTTON_WIDTH))) {
				if ((yTouchCoord > MAIN_MENU_BUTTON_STARTY) && (yTouchCoord < (MAIN_MENU_BUTTON_STARTY+MAIN_MENU_BUTTON_HEIGHT))) {
					changingProgram = 1;
					return;
				}
			}
			moveThroughMaze((volatile unsigned int)xTouchCoord, (volatile unsigned int)yTouchCoord);
			break;
		case SNAKE_GAME_ID:
			if (inSnakeIntro) {
				if ((xTouchCoord > OK_BUTTON_RC_STARTX) && (xTouchCoord < (OK_BUTTON_RC_STARTX+OK_BUTTON_RC_WIDTH))) {
					if ((yTouchCoord > OK_BUTTON_RC_STARTY) && (yTouchCoord < (OK_BUTTON_RC_STARTY+OK_BUTTON_RC_HEIGHT))) {
						inSnakeIntro = 0;
						return;
					}
				}
			}
			
			if ((xTouchCoord > MAIN_MENU_BUTTON_STARTX) && (xTouchCoord < (MAIN_MENU_BUTTON_STARTX+MAIN_MENU_BUTTON_WIDTH))) {
				if ((yTouchCoord > MAIN_MENU_BUTTON_STARTY) && (yTouchCoord < (MAIN_MENU_BUTTON_STARTY+MAIN_MENU_BUTTON_HEIGHT))) {
					changingProgram = 1;
					return;
				}
			}

			// Update Direction ///////////////////////////////////////////////////////////////
			snakeHead->oldDir = snakeHead->dir;
			
			if(snakeHead->dir == UP || snakeHead->dir == DOWN) {
				if (xTouchCoord > SNAKE_RIGHTBUTTON_BOUNDARY) snakeHead->dir = RIGHT;
				else if (xTouchCoord < SNAKE_LEFTBUTTON_BOUNDARY) snakeHead->dir = LEFT;
			}
			else {
				if (xTouchCoord > SNAKE_LEFTBUTTON_BOUNDARY && xTouchCoord < SNAKE_RIGHTBUTTON_BOUNDARY) {
					if (yTouchCoord > SNAKE_UPDOWNBUTTON_BOUNDARY) snakeHead->dir = DOWN;
					else snakeHead->dir = UP;
				}
			}
			
			//if(xTouchCoord < SNAKE_LEFTBUTTON_BOUNDARY) {			// Moving Left
				//if (snakeHead->dir == UP || snakeHead->dir == DOWN) snakeHead->dir = LEFT;
			//}
				
			//else if(xTouchCoord > SNAKE_RIGHTBUTON_BOUNDARY) {		// Moving Right
				//if (snakeHead->dir == UP || snakeHead->dir == DOWN) snakeHead->dir = RIGHT;
			//}
							//
			//else if (yTouchCoord < SNAKE_UPDOWNBUTTON_BOUNDARY) {		// Moving Up
				//if (snakeHead->dir == LEFT || snakeHead->dir == RIGHT) snakeHead->dir = UP;
			//}
			//
			//else {														// Moving Down
				//if (snakeHead->dir == LEFT || snakeHead->dir == RIGHT) snakeHead->dir = DOWN;
			//}
		//
			//snakeHead->oldDir = snakeHead->dir;
			//if(snakeHead->dir == UP || snakeHead->dir == DOWN) {
				//if (xTouchCoord > snakeHead->x) snakeHead->dir = RIGHT;
				//else snakeHead->dir = LEFT;
			//}
			//else {
				//if (yTouchCoord > snakeHead->y) snakeHead->dir = DOWN;
				//else snakeHead->dir = UP;
			//}		
			//////////////////////////////////////////////////////////////////////////////////
			break;
		case TABLE_READY_PG_ID:
			if ((xTouchCoord > OK_BUTTON_BC_STARTX) && (xTouchCoord < (OK_BUTTON_BC_STARTX+OK_BUTTON_BC_WIDTH))) {
				if ((yTouchCoord > OK_BUTTON_BC_STARTY) && (yTouchCoord < (OK_BUTTON_BC_STARTY+OK_BUTTON_BC_HEIGHT))) {
					//currentProgram = MAIN_MENU_ID;
					//changingProgram = 1;
					resumingProgram = 1;
					tableReady = 0;
					soundStop();
					vibrate_pulsed_stop();
				}
			}
			break;
		case PAGING_USER_PG_ID:
			if ((xTouchCoord > OK_BUTTON_BC_STARTX) && (xTouchCoord < (OK_BUTTON_BC_STARTX+OK_BUTTON_BC_WIDTH))) {
				if ((yTouchCoord > OK_BUTTON_BC_STARTY) && (yTouchCoord < (OK_BUTTON_BC_STARTY+OK_BUTTON_BC_HEIGHT))) {
					resumingProgram = 1;
					pagingUser = 0;
					soundStop();
					vibrate_pulsed_stop();
				}
			}
			break;
		case OOR_PG_ID:
			if ((xTouchCoord > OK_BUTTON_BC_STARTX) && (xTouchCoord < (OK_BUTTON_BC_STARTX+OK_BUTTON_BC_WIDTH))) {
				if ((yTouchCoord > OK_BUTTON_BC_STARTY) && (yTouchCoord < (OK_BUTTON_BC_STARTY+OK_BUTTON_BC_HEIGHT))) {
					resumingProgram = 1;
					outOfRange = 0;
					soundStop();
					vibrate_pulsed_stop();
				}
			}
			break;
	}
	debug = 3;
/*	
	if (currentProgram == MAZE_GAME_ID) {
		touchSenseReset();
	}
*/
}

// After Debouncing period, detect touches again
ISR(TCC1_CCB_vect) {
	TCC1_CTRLA = 0x00;	// Stop timer
	TCC1_CTRLFSET = 0x08;	//Restart timer
	if (!changingProgram && !resumingProgram) {
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

// Frankie ------------------------------------------------------------------

void makeSineWave() {
	int iii;
	//unsigned int temp;
	double sinC[49];
	//unsigned int sinArray[50];
	for(iii=0;iii<50;iii++){//construct sinArray
		sinC[iii]=100+100*sin(2*M_PI*iii/(50));
		sinArray[iii]=(unsigned int)(sinC[iii]);
		//	sinArray[iii]=(unsigned int)100*sin(2*M_PI*iii/(50));
	}
}

void SPIC_init(){
	// Set MOSI,SS and SCK, CE output.
	PORTC_DIRSET=0xB6;
	// Enable SPI, Master, set clock rate fck/128
	SPIC_CTRL=0x50;//50
	
	PORTC_OUTSET =0x10;// set SS[CSN] High
	PORTC_OUTCLR = 0x02;//set CE of TR to 0
}

void SPID_init(){
	// Set MOSI,SS and SCK as output.
	PORTD_DIRSET=0xB0;
	// Enable SPI, Master, set clock rate fck/128
	//SPID_CTRL=0x50;//enable SPID, master mode
	SPID_CTRL=0xD0;//Enable SPI, master mode,  x2 sck freq.
	PORTD_OUTSET =0x10;// set SS[CSN] High
}

void COM_INIT(){
	//Write to  CONFIG reg:mask all interrupts, Power up , CRC 1 byte, TX mode
	//Write to  reg, 0x3A: pwr_up=1,
	COM_WRITE(1,0x00,0x3A,0,0,0,0);
	_delay_ms(1000);
	COM_WRITE(1,0x00,0x38,0,0,0,0);
	_delay_ms(1000);
	COM_WRITE(1,0x00,0x3A,0,0,0,0);
	//Enable RX_DR Interrupt via IRQ pin. low true if received data (C0, pin16)
	sei();
	PMIC_CTRL|=0x03;
	PORTC_INTCTRL=0x03;//set as high priority.
	PORTC_INT0MASK=0x01;//C0 set
	PORTC_PIN0CTRL |=0x02;//to sense falling edge(0x02)
	//Write RX addr
	COM_WRITE(5,0x0A,0xFF,0x12,0x13,0x14,0x15);
	COM_WRITE(5,0x0B,unitNumber,0x12,0x13,0x14,0x15);
	//Write TX addr
	COM_WRITE(5,0x10,0xBB,0x89,0x87,0x86,0x85);
	//DEBUG:Setpower to low, ch17
	COM_WRITE(1,0x06,0x26,0,0,0,0);
	COM_WRITE(1,0x05,0x10,0,0,0,0);
	/////////////////////DEBUG////////////////////
	//COM_FLUSH_RX();
	//Reset RX_DR Pin via RX_DR bit in status
	volatile unsigned int status = COM_STATUS();
	volatile unsigned int temp= status | 0x40;//sets RX_DR to 1
	COM_WRITE(1,0x07,temp,0,0,0,0);
	////////////////////////////////////////
}

unsigned int COM_STATUS(){
	//send CMD word
	unsigned int status;
	PORTC_OUTCLR=0x10;//Start transmission, set CSN low
	SPIC_DATA =0xFF;
	while(!(SPIC_STATUS & (1<<7)));//Check SPIC_STATUS_IF to see if transmission completed.
	PORTC_OUTSET=0x10; //set CSN high
	//recieve data
	status=SPIC_DATA;
	return status;
}

unsigned int COM_READ(unsigned int regValue){
	//send CMD word
	unsigned int dataRead;
	PORTC_OUTCLR=0x10;//Start transmission, set CSN low
	SPIC_DATA =regValue;
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	//recieve data
	SPIC_DATA=0xFE;//junk data
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	dataRead=SPIC_DATA;
	PORTC_OUTSET=0x10; //set CSN high
	return dataRead;
};

/*
void COM_READ_LARGE(int numBytes, unsigned int regValue, unsigned int &byte1,unsigned int &byte2,unsigned int &byte3,unsigned int &byte4,unsigned int &byte5){
	//send CMD word
	PORTC_OUTCLR=0x10;//Start transmission, set CSN low
	SPIC_DATA =regValue;
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	if(numBytes >=1){
		SPIC_DATA=0xFE;//junk data
		while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
		byte1=SPIC_DATA;
	}
	if(numBytes >=2){
		SPIC_DATA=0xFE;//junk data
		while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
		byte2=SPIC_DATA;
	}
	if(numBytes >=3){
		SPIC_DATA=0xFE;//junk data
		while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
		byte3=SPIC_DATA;
	}
	if(numBytes >=4){
		SPIC_DATA=0xFE;//junk data
		while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
		byte4=SPIC_DATA;
	}
	if(numBytes == 5){
		SPIC_DATA=0xFE;//junk data
		while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
		byte5=SPIC_DATA;
	}
	PORTC_OUTSET=0x10; //set CSN high
}
*/

void COM_WRITE(int numBytes,unsigned int regValue, unsigned int byte1, unsigned int byte2, unsigned int byte3, unsigned int byte4, unsigned int byte5){
	//send CMD word
	regValue=regValue | 0x20;
	PORTC_OUTCLR=0x10;//Start transmission, set CSN low
	SPIC_DATA =regValue;
	//while(!(SPSR0 & (1<<SPIF0)));
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	//Transmit bytes, LSB to MSB
	if(numBytes >=1){
		SPIC_DATA =byte1;
		while(!(SPIC_STATUS & (1<<7)));
	}
	if(numBytes >=2){
		SPIC_DATA =byte2;
		while(!(SPIC_STATUS & (1<<7)));
	}
	if(numBytes >=3){
		SPIC_DATA =byte3;
		while(!(SPIC_STATUS & (1<<7)));
	}
	if(numBytes >=4){
		SPIC_DATA =byte4;
		while(!(SPIC_STATUS & (1<<7)));
	}
	if(numBytes == 5){
		SPIC_DATA =byte5;
		while(!(SPIC_STATUS & (1<<7)));
	}
	PORTC_OUTSET=0x10; //set CSN high
	
}

void COM_WRITE_PAYLOAD(unsigned int payload){//MUST BE IN TX MODE, 8 bit message. change to higher but  datatype later
	//send CMD word
	PORTC_OUTCLR=0x10;//Start transmission, set CSN low
	SPIC_DATA =0xA0;
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	//Transmit bytes, LSB to MSB
	SPIC_DATA =payload;
	while(!(SPIC_STATUS & (1<<7)));
	PORTC_OUTSET=0x10; //set CSN high
	
}

unsigned int COM_READ_PAYLOAD(){//MUST be in RX mode. Read payload from RX FIFO
	volatile unsigned int payload, status,temp;
	//SET CE LOW
	PORTC_OUTCLR = 0x02;//set CE to be 0;
	//COM_READ(0x17,temp);//DEBUG
	
	//send CMD word
	PORTC_OUTCLR=0x10;//Start transmission, set CSN low
	SPIC_DATA =0x61;
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	//recieve data
	SPIC_DATA=0xFE;//junk data
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	payload=SPIC_DATA;
	PORTC_OUTSET=0x10; //set CSN high
	
	//COM_READ(0x17,temp);//DEBUG
	temp=0x00;
	
	//Reset RX_DR Pin via RX_DR bit in status
	status = COM_STATUS();
	temp= status | 0x40;//sets RX_DR to 1
	COM_WRITE(1,0x07,temp,0,0,0,0);
	//COM_STATUS(status);//DEBUG:get status
	//set CE high to re-enter active RX mode
	PORTC_OUTSET = 0x02; 	//set CE to be 1
	return payload;
	
}

void COM_FLUSH_TX(){
	//send CMD word
	PORTC_OUTCLR=0x10;//Start transmission, set CSN low
	SPIC_DATA =0xE1;
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	PORTC_OUTSET=0x10; //set CSN high
}

void COM_FLUSH_RX(){
	//send CMD word
	PORTC_OUTCLR=0x10;//Start transmission, set CSN low
	SPIC_DATA =0xE2;
	while(!(SPIC_STATUS & (1<<7)));// Wait for transmission to complete
	PORTC_OUTSET=0x10; //set CSN high
}

void COM_TX(unsigned int payload){//8 bit message, change to higher bit length datatype later
	unsigned int temp, status,wait=1;
	PORTC_OUTCLR = 0x02;//SET CE to be 0
	//Set CONGIF bit PRIM_RX to low.
	temp = COM_READ(0x00);
	temp &= 0xFE;
	COM_WRITE(1,0x00,temp,0,0,0,0);
	
	//Write TX addr(done in init)
	//Write payload
	COM_WRITE_PAYLOAD(payload);
	//Step3:Pulse CE for 10 seconds
	PORTC_OUTSET = 0x02;//set CE to be 1;
	_delay_us(50);//min=10 us
	PORTC_OUTCLR = 0x02;//set CE to be 0;
	
	//Check for ACK (TX_DS=1 is successful) in status register, If not received( TX_DS=0 and MAX_RT =1) flush payload and reset TX_DS and MAX_RT by writing 1.
	do
	{
		status = COM_STATUS();//Read Status Register
		temp=status & 0x30;
		if(temp == 0x20){//TX_DS=1, YAY! Clear TX_DS.
			wait=0;
			temp= status | 0x20;//sets TX_DS=1 to clear
			COM_WRITE(1,0x07,temp,0,0,0,0);
			//COM_STATUS(status);//DEBUG:get status
		}
		else if(temp == 0x10){//MAX_RT=1 =(
			//Ideally, input something to deal with this error. Instead, Flush payload and clear status
			wait=0;
			COM_FLUSH_TX();
			temp= status | 0x10;//sets MAX_RT to 1
			COM_WRITE(1,0x07,temp,0,0,0,0);
			//COM_STATUS(status);//DEBUG:get status
		}
	} while (wait==1);
	
}

void COM_RX_MODE(){//Enters RX mode, setup datapipe0
	unsigned int temp;
	//unsigned int temp1,temp2,temp3,temp4,temp5;//DEBUG
	//COM_READ(0x17);
	//Set payload length in RX_PW_PX register
	COM_WRITE(1,0x11,0x01,0,0,0,0);//Enables datapipe0, sets length to 8.
	COM_WRITE(1,0x12,0x01,0,0,0,0);//Enables datapipe1, sets length to 8.
	//Only use datapipe0, datapipe1
	COM_WRITE(1,0x02,0x03,0,0,0,0);
	//Ensure addr set properly, datapipe enabled(both done in init)
	
	//Set CONGIF bit PRIM_RX to 1. Set RX_IRQ enabled.
	temp = COM_READ(0x00);
	temp |= 0x01;
	temp &= 0x3F;
	//temp=0x3B;
	COM_WRITE(1,0x00,temp,0,0,0,0);
	
	//DEBUG
	tempa = COM_STATUS();
	tempa = COM_READ(0x11);
	tempa = COM_READ(0x13);
	tempa = COM_READ(0x12);
	tempa = COM_READ(0x00);
	tempa = COM_READ(0x17);
	tempa = COM_STATUS();
	//COM_READ_LARGE(5,0x0A,temp1,temp2,temp3,temp4,temp5);
	//COM_READ_LARGE(5,0x10,temp1,temp2,temp3,temp4,temp5);
	////COM_READ()
	//COM_FLUSH_TX();
	//COM_FLUSH_RX();
	PORTC_OUTSET = 0x02;//set CE to be 1 This enters active RX mode.
}

void TIMER_CONFIG(){
	unsigned int temp;
	temp= GetNoteFreq(49);
	TCC0_PER=0xFFFF;
	TCC0_CCA=temp;
	currentFreq=temp;
	//TCC0_PER=160;// Value:Freq 3200:200, 440:1455 23703:27,160:4k for 50 samples
	sei();
	
	PMIC_CTRL|=0x02;//enable medium interrupts. pg.100
	TCC0_INTCTRLB=0x02;//set CompareIntrrupt A as medium
	TCC0_INTCTRLA=0x02;//overflow interrupt set as medium priority. pg.138
	//TCC0_CTRLB=0x10;//enable WG mode, no pin overrides.
	//TCC0_CTRLA=0x01;// Prescale is clk/1, enables timer
	
	makeSineWave();
}

unsigned int GetNoteFreq(unsigned int noteID){
	//const unsigned int clkFreq=32000000;
	//const unsigned int preScaler=1;
	//const unsigned int sampleNum = 50;
	if(noteID > 88){
		noteID =0;
	}
	unsigned int ret;
	ret=(unsigned int)floor((clkFreq/preScaler)/((freqArray[noteID]*sampleNum)));
	return ret;
}

void soundPlay(){
	if (!soundOn) return;
	
	volatile unsigned int noteVal,countVal;
	unsigned int temp;
	unsigned int outputData=0,temp2;
	/*16 bits: 
	//7:0 determine note (e.x. decimal 81 would be encoded as 0x51
	//11:8 determine note length, decimal 1 to 8. Length example:1 eight note is 1, 1 hole note is 8.
	//
	//using note length of zero will terminate sound. Using invalid note value (not between 0 and 88) will result in note being skipped.
	*/	
	debug = musicArray[musicArrayCount];
	songPlaying = 1;	
	noteVal=(unsigned int) (musicArray[musicArrayCount] & 0xFF);
	countVal=(unsigned int)(musicArray[musicArrayCount]>>8);
	musicArrayCount++;
	
	if (countVal == 0 && currentSongLoop == 1 && loopsLeft > 0) {
		musicArrayCount = currentSongStartIndex;
		noteVal=(unsigned int) (musicArray[musicArrayCount] & 0xFF);
		countVal=(unsigned int)(musicArray[musicArrayCount]>>8);
		musicArrayCount++;
		loopsLeft--;
	}
	
	if ((countVal !=0)){//if count valid
		if(noteVal == 0) {
			silentNote = 1;
			/*// Set DAC to 0 ///////////////////////
			outputData=0;
			temp2=(0xF000 | (outputData<<2));
			temp=(char)((temp2>>8));
			//Start transmission
			PORTD_OUTCLR = 0x10;//SS low
			SPID_DATA =temp;
			// Wait for transmission 1  to complete
			while(!(SPID_STATUS & (1<<7)));
			temp=(char)((temp2 & 0x00FF));
			SPID_DATA =temp;
			// Wait for transmission 2 to complete
			while(!(SPID_STATUS & (1<<7)));
			PORTD_OUTSET = 0x10;//SS high
			*///////////////////////////////////////////
			TCC0_INTCTRLB = 0x00;	// Disable CCA interrupt (silent note)
			TCC0_CTRLA = 0x01;	// Start timer
		}
		else if(noteVal<=88){
			if (!(currentProgram == TABLE_READY_PG_ID || currentProgram == PAGING_USER_PG_ID || currentProgram == OOR_PG_ID)) {
				// Disable Touch Sensing ----------------------------------
				PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
				PORTA_INTCTRL = 0x00;	// Disable PORTA Interrupts
				//--------------------------------------------------------
			}			
			noteLength=countVal;
			temp= GetNoteFreq(noteVal);
			TCC0_CCA=temp;
			currentFreq=temp;
			TCC0_CTRLB=0x10;
			TCC0_CTRLA=0x01;
		}//end noteVal
		
	}//END If count valid
	else{
	soundOn = 0;
	outputData=0;
	temp2=(0xF000 | (outputData<<2));
	temp=(char)((temp2>>8));
	//Start transmission
	PORTD_OUTCLR = 0x10;//SS low
	SPID_DATA =temp;
	// Wait for transmission 1  to complete
	while(!(SPID_STATUS & (1<<7)));
	temp=(char)((temp2 & 0x00FF));
	SPID_DATA =temp;
	// Wait for transmission 2 to complete
	while(!(SPID_STATUS & (1<<7)));
	PORTD_OUTSET = 0x10;//SS high
	
	
	//PORTC_OUTCLR=0x04;//DEBUG
	//musicArrayCount=0x00;
	
	songPlaying = 0;
	touchSenseReset();
	
	}
}

ISR(PORTC_INT0_vect){//If payload recieved
	unsigned int rxData;
	rxData = COM_READ_PAYLOAD();
	//if(rxData != 0 ){
		//if (rxData != 0xFF) {
			//*placeInLine = rxData;
			//calculateDigits(placeInLine, PILDigits);
			//PILChanged = 1;
			////tft_print_square(200, 120, TFT_GREEN, 20);
			//makeSineWave();
			//soundPlay();
		//}
	//}
	//else{
		//// We're being paged
		//musicArrayCount=7;
		//makeSineWave();
		//soundPlay();
	//}
	
	if(rxData < 0x9B) {
		*placeInLine = rxData;
		calculateDigits(placeInLine, PILDigits);
		PILChanged = 1;
		
		if(*placeInLine == 0) {
			tableReady = 1;
		}
		
		#ifdef HAVE_BASE
		TCD0_CTRLFSET = 0x08;		// restart OOR timer
		TCD0_INTCTRLB = 0x01;		// CCA interrupt enabled at low priority
		#endif
	}
	else {
		if (rxData == 0xFE){
			pagingUser = 1;
		}
		else if (rxData == 0xFF) {
			timeSincePing = 0;
		}
		else {
			if (*placeInLine > rxData - 0x9B) {
				debug = *placeInLine;
				(*placeInLine)--;
				calculateDigits(placeInLine, PILDigits);
				PILChanged = 1;
				debug = *placeInLine;
			}
		}	
	}
	
	timeSincePing = 0;
	
	COM_FLUSH_RX();
	
	//COM_INIT();
	//COM_RX_MODE();
}

ISR(TCC0_CCA_vect){
	if (!soundOn) return;
	unsigned int outputData=0,temp2,temp, Vpp=1024;
	outputData =  Vpp/100/2*sinArray[id1];
	id1++;
	TCC0_CTRLA=0x00;//pause timer
	TCC0_CCA += currentFreq;//increment CCA
	TCC0_CTRLA=0x01;//resume timer
	//if(id1%2==0){
	//	outputData=512;
	//PORTC_OUTCLR =0x04;
	//}
	temp2=(0xF000 | (outputData<<2));
	temp=(char)((temp2>>8));
	//Start transmission
	PORTD_OUTCLR = 0x10;//SS low
	SPID_DATA =temp;
	// Wait for transmission 1  to complete
	while(!(SPID_STATUS & (1<<7)));
	temp=(char)((temp2 & 0x00FF));
	SPID_DATA =temp;
	// Wait for transmission 2 to complete
	while(!(SPID_STATUS & (1<<7)));
	PORTD_OUTSET = 0x10;//SS high
	
	if(id1 >=50){
		id1=0;
	}
}

ISR(TCC0_OVF_vect){//ifv note done playing, stop. call back to function to determine next note info.
	if (!soundOn) return;
	//unsigned int temp;
	subCount1++;
	if(subCount1>=3*noteLength){//increment every 0.5 seconds hz(120 bpm base when prescale is 1)
		//TCC0_CTRLA=0x00;
		subCount1=0;
		subCount3++;
		//PORTC_OUTTGL =0x04;//DEBUG
		if(subCount3>88){//change to 88? check array for missing values
			subCount3=1;
		}
		//currentFreq= GetNoteFreq(subCount3);
		//currentFreq=temp;
		//TCC0_CCA=currentFreq;
		//PMIC_CTRL=0x00;//enable medium interrupts. pg.100
		TCC0_CTRLA=0x00;//disable timer.
		//PMIC_CTRL=0x00;//enable medium interrupts. pg.100
		TCC0_CTRLB=0x00;
		//TCC0_CTRLA=0x01;
		if(silentNote == 1) {
			TCC0_INTCTRLB = 0x02;	// Reenable CCA interrupt
			silentNote = 0;
		}
		soundPlay();//Swap above lines with this.
	}
}

void soundStop() {
	TCC0_CTRLA=0x00;//disable timer.
	TCC0_CTRLB=0x00;
	soundOn = 0;
}

// ---------------------------------------------------------------------------