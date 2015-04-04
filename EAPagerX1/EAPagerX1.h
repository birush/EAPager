#ifndef EAPagerX1_H
#define EAPagerX1_H


void clock_init(void);
void tft_init(void);
void tft_write_command(char);
void tft_write_data16(unsigned int);
void tft_write_data8(char);
void setWindow(unsigned int, unsigned int, unsigned int, unsigned int);
void tft_print_image(unsigned char, unsigned int, unsigned int, unsigned int, unsigned int);
void tft_print_square(unsigned int, unsigned int, unsigned int, unsigned char);
void tft_print_blank_background(unsigned int);
void setWindow(unsigned int, unsigned int, unsigned int, unsigned int);
void touchInit();
void touchSenseInit();
void measureTouchCoordinates();
void adc_enable();
void adc_take_sample(char);
void debug1Function(void);
void alternateColors(void);
void touchSenseReset(void);
void vibrate_init(void);
void vibrate(unsigned int);
void vibrate_pulsed_start(unsigned int, unsigned int, unsigned char);
void vibrate_pulsed_stop(void);

// Frankie's Headers ----------------------------------------------------------------------------------------------------------
void COM_WRITE(int ,unsigned int , unsigned int , unsigned int , unsigned int , unsigned int , unsigned int);
unsigned int COM_STATUS(void);
unsigned int COM_READ(unsigned int);
void COM_FLUSH_TX();
void COM_FLUSH_RX();
void COM_RX_MODE();
void COM_INIT();
unsigned int COM_READ_PAYLOAD(void);
void COM_WRITE_PAYLOAD(unsigned int);
void COM_TX(unsigned int);
//void COM_READ_LARGE(int numBytes, unsigned int regValue, unsigned int&,unsigned int&,unsigned int&,unsigned int&,unsigned int&);
void clock_init();
unsigned int GetNoteFreq(unsigned int);
void SoundPlay(void);
void SPID_init(void);
void SPIC_init(void);
void makeSineWave(void);
void TIMER_CONFIG(void);
// -----------------------------------------------------------------------------------

volatile unsigned char currentProgram;
volatile unsigned char changingProgram;
unsigned int debug, debug1;

#endif



#ifndef EAPagerX1_Constants_H
#define EAPagerX1_Constants_H

//======= IMAGE CONSTANTS ========================
#define HS_BACKGROUND_IMAGE_ID 255
#define FULL_IMAGE_WIDTH 400
#define FULL_IMAGE_HEIGHT 240

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH_END 799
#define SCREEN_HEIGHT_END 479

// Mazes Button //////////////////////
#define HS_MAZES_BUTTON_IMAGE_ID 254
#define HS_MAZES_BUTTON_STARTX 48
#define HS_MAZES_BUTTON_STARTY 68
/////////////////////////////////////

// Snake Button /////////////////////
#define HS_SNAKE_BUTTON_IMAGE_ID 253
#define HS_SNAKE_BUTTON_STARTX 248
#define HS_SNAKE_BUTTON_STARTY 68
/////////////////////////////////////


#define HS_BUTTON_IMAGE_WIDTH 104
#define HS_BUTTON_IMAGE_HEIGHT 104


// Main Menu Button //////////////////
#define MAIN_MENU_BUTTON_ID 252
#define MAIN_MENU_BUTTON_WIDTH 72
#define MAIN_MENU_BUTTON_HEIGHT 16
#define MAIN_MENU_BUTTON_STARTX 325
#define MAIN_MENU_BUTTON_STARTY 220

///////////////////////////////////////
#define MAZE0_IMAGE_ID 10
#define MAZE1_IMAGE_ID 11
#define PIL_STARTX 245
#define PIL_STARTY 210
#define PIL_SPACING 14
#define NUM0_ID 0
#define NUM1_ID 1
#define NUM2_ID 2
#define NUM3_ID 3
#define NUM4_ID 4
#define NUM5_ID 5
#define NUM6_ID 6
#define NUM7_ID 7
#define NUM8_ID 8
#define NUM9_ID 9
#define NUM_WIDTH 16
#define NUM_HEIGHT 16
// =================================

//===== Game Constants ============
#define MAIN_MENU_ID 0
#define MAZE_GAME_ID 1
#define SNAKE_GAME_ID 2
//=================================

// 16 bit Color Definitions =============
#define TFT_BLACK 0x0000
#define TFT_WHITE 0xFFFF
#define TFT_RED 0xF800
#define TFT_GREEN 0x07E0
#define TFT_BLUE 0x001F
#define TFT_ORANGE 0xFBE0
#define TFT_CASE_BLUE 0x039F
#define TFT_HS_BACKGROUND_COLOR TFT_ORANGE//0x33FF
#define TFT_HS_BUTTON_COLOR TFT_CASE_BLUE//0x76EA
// ======================================

// Timing Constants ---------------------------
#define VB_ONE 31250
#define VB_THREE_FOURTHS 23438
#define VB_HALF 15625
#define VB_THREE_EIGHTHS 11719
#define VB_FOURTH 7813
#define VB_EIGHTH 3906
#define VB_SIXTEENTH 1953
// --------------------------------------------
#endif		// EAPagerX1_Constants_H



#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

void playSnakeGame(void);
void makeNewFood(void);
unsigned int getRandCoord(unsigned int, unsigned char);

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4
#define SPEED_NORMAL 15625
#define X_COORD_TYPE 0
#define Y_COORD_TYPE 1
#define MIN_SNAKE_X_COORD 20
#define MIN_SNAKE_Y_COORD 20
#define MAX_SNAKE_X_COORD (FULL_IMAGE_WIDTH-MIN_SNAKE_X_COORD)
#define MAX_SNAKE_Y_COORD (FULL_IMAGE_HEIGHT-MIN_SNAKE_Y_COORD)

unsigned int foodSeed;

typedef struct snakeNode snakeNode;
struct snakeNode {
	volatile unsigned int x;
	volatile unsigned int y;
	volatile unsigned char dir;
	volatile unsigned char oldDir;
	volatile snakeNode *nextSnakeNode;
};

typedef struct foodPiece foodPiece;
struct foodPiece {
	unsigned int x;
	unsigned int y;
};

volatile snakeNode *head;
volatile snakeNode *tail;
volatile snakeNode *nextBend;
volatile snakeNode *newestBend;
volatile snakeNode *tempSnakeNode;

foodPiece food;

unsigned int startX;
unsigned int startY;
unsigned int gameSpeed;
unsigned char blockSize;
unsigned int backGroundColor;
unsigned int snakeColor;
unsigned int foodColor;
unsigned int tailXOffset;


#endif //SNAKE_GAME_H_
