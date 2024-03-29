//#define BLUE1 1
//#define HAVE_BASE 1

#ifndef BLUE1
#define ORANGE2 2
#endif

#ifndef EAPagerX1_H
#define EAPagerX1_H

#define F_CPU 32000000

// Song IDs --------------------------------
#define TRILL_SONG_ID 0
#define WTOPENING_SONG_ID 13
#define WTVERSE_SONG_ID 37
#define ALERT_SONG_ID 121
#define ATTN_SONG_ID 127
#define GAMEOVER_SONG_ID 131
#define SUCCESS_SONG_ID 140
//------------------------------------------

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
void printDigits(unsigned int, volatile unsigned char[3], unsigned int, unsigned int);
void calculateDigits(volatile unsigned char*, volatile unsigned char[3]);
void pageUser(void);
void showOORMessage(void);
void tableReadyMessage(void);

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
void soundPlay(void);
void SPID_init(void);
void SPIC_init(void);
void makeSineWave(void);
void TIMER_CONFIG(void);
void soundStop(void);
void playSong(unsigned int, unsigned char, unsigned char);
// -----------------------------------------------------------------------------------

volatile unsigned char currentProgram; 
unsigned int debug, debug1;
//volatile unsigned char highDigit, lowDigit;
volatile unsigned char* placeInLine;
volatile unsigned char PILChanged, resumingProgram, changingProgram, pagingUser, outOfRange, soundOn, tableReady;
volatile unsigned char PILDigits[4];
volatile unsigned char SGSDigits[4];
volatile unsigned char touchDebounceDelay;
//volatile unsigned int tcd0_prevPER, tcd0_prevCTRLA;
volatile unsigned char timeSincePing;

#endif



#ifndef EAPagerX1_Constants_H
#define EAPagerX1_Constants_H

//======= IMAGE CONSTANTS ========================
#define HS_BACKGROUND_IMAGE_ID 240
#define FULL_IMAGE_WIDTH 400
#define FULL_IMAGE_HEIGHT 240

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH_END 799
#define SCREEN_HEIGHT_END 479

// Mazes Button //////////////////////
#define HS_MAZES_BUTTON_IMAGE_ID 100
#define HS_MAZES_BUTTON_STARTX 48
#define HS_MAZES_BUTTON_STARTY 68
/////////////////////////////////////

// Snake Button /////////////////////
#define HS_SNAKE_BUTTON_IMAGE_ID 101
#define HS_SNAKE_BUTTON_STARTX 248
#define HS_SNAKE_BUTTON_STARTY 68
/////////////////////////////////////


#define HS_BUTTON_IMAGE_WIDTH 104
#define HS_BUTTON_IMAGE_HEIGHT 104


// Main Menu Button //////////////////
#define MAIN_MENU_BUTTON_ID 102
#define MAIN_MENU_BUTTON_WIDTH 72
#define MAIN_MENU_BUTTON_HEIGHT 16
#define MAIN_MENU_BUTTON_STARTX 298
#define MAIN_MENU_BUTTON_STARTY 210
///////////////////////////////////////

// Snake "Game Over" ////////////////
#define SNAKE_GAME_OVER_ID 103
#define SNAKE_GAME_OVER_WIDTH 296
#define SNAKE_GAME_OVER_HEIGHT 140
#define SNAKE_GAME_OVER_STARTX 52
#define SNAKE_GAME_OVER_STARTY 35
/////////////////////////////////////

// Table Ready Message ///////////////
#define TABLE_READY_IMAGE_ID 104
#define TABLE_READY_WIDTH 400
#define TABLE_READY_HEIGHT 50
#define TABLE_READY_STARTX 0
#define TABLE_READY_STARTY 95
//////////////////////////////////////

// OK Button - Bottom Centered ///////
#define OK_BUTTON_BC_ID 105
#define OK_BUTTON_BC_WIDTH 88
#define OK_BUTTON_BC_HEIGHT 30
#define OK_BUTTON_BC_STARTX 155
#define OK_BUTTON_BC_STARTY 167
//////////////////////////////////////

// OK Button /////////////////////////
#define OK_BUTTON_RC_ID OK_BUTTON_BC_ID
#define OK_BUTTON_RC_WIDTH OK_BUTTON_BC_WIDTH
#define OK_BUTTON_RC_HEIGHT OK_BUTTON_BC_HEIGHT
#define OK_BUTTON_RC_STARTX 261
#define OK_BUTTON_RC_STARTY 128
//////////////////////////////////////

// Paging User Message ///////////////
#define PAGING_USER_IMAGE_ID 250
#define PAGING_USER_WIDTH 400
#define PAGING_USER_HEIGHT 240
#define PAGING_USER_STARTX 0
#define PAGING_USER_STARTY 0
//////////////////////////////////////

// Out Of Range Message ///////////////
#define OOR_IMAGE_ID 251
#define OOR_WIDTH 400
#define OOR_HEIGHT 240
#define OOR_STARTX 0
#define OOR_STARTY 0
//////////////////////////////////////

// Maze Solved Message ///////////////
#define MAZE_SOLVED_IMAGE_ID 108
#define MAZE_SOLVED_IMAGE_WIDTH SNAKE_GAME_OVER_WIDTH
#define MAZE_SOLVED_IMAGE_HEIGHT SNAKE_GAME_OVER_HEIGHT
#define MAZE_SOLVED_IMAGE_STARTX SNAKE_GAME_OVER_STARTX
#define MAZE_SOLVED_IMAGE_STARTY SNAKE_GAME_OVER_STARTY
/////////////////////////////////////

///////////////////////////////////////
#define MAZE0_IMAGE_ID 220
#define MAZE0_PATH_ID 200
#define MAZE1_IMAGE_ID 221
#define MAZE1_PATH_ID 201
#define MAZE2_IMAGE_ID 222
#define MAZE2_PATH_ID 202
#define MAZE3_IMAGE_ID 223
#define MAZE3_PATH_ID 203
#define MAZE4_IMAGE_ID 224
#define MAZE4_PATH_ID 204
#define PIL_STARTX 178
#define PIL_STARTY 209
#define DIGIT_OFFSET1 14
#define DIGIT_OFFSET2 28
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

// Snake Game Background/////////////
#define SG_BACKGROUND_IMAGE_ID 241
/////////////////////////////////////

// Maze Intro ///////////////////////
#define MG_INTRO_IMAGE_ID 242
/////////////////////////////////////

// Snake Intro //////////////////////
#define SG_INTRO_IMAGE_ID 243
#define SG_INTRO_HS_STARTX 114
#define SG_INTRO_HS_ENDX 137
#define SG_INTRO_HS_STARTY 143
#define SG_INTRO_VS_STARTX 320
#define SG_INTRO_VS_STARTY 107
#define SG_INTRO_VS_ENDY 127
#define SG_INTRO_S_WIDTH 3
#define SG_INTRO_S_LENGTH 21 
/////////////////////////////////////

// Snake Score Constants ////////////
#define SGS_STARTX 262
#define SGS_STARTY 114
////////////////////////////////////

// =================================

//===== Game Constants ============
#define MAIN_MENU_ID 0
#define MAZE_GAME_ID 1
#define SNAKE_GAME_ID 2
#define TABLE_READY_PG_ID 3
#define PAGING_USER_PG_ID 4
#define OOR_PG_ID 5
//=================================

// 16 bit Color Definitions =============
#define TFT_BLACK 0x0000
#define TFT_WHITE 0xFFFF
#define TFT_RED 0xF800
#define TFT_GREEN 0x07E0
#define TFT_BLUE 0x001F
#define TFT_ORANGE 0xFBE0
#define TFT_CASE_BLUE 0x039F
#define TFT_CASE_ORANGE 0xFB00

#ifdef BLUE1
#define TFT_HS_BACKGROUND_COLOR TFT_CASE_ORANGE//0x33FF
#define TFT_HS_BUTTON_COLOR TFT_CASE_BLUE//0x76EA
#define GAME_BACKGROUND_COLOR TFT_CASE_ORANGE
#define GAME_FOREGROUND_COLOR TFT_CASE_BLUE
#endif

#ifdef ORANGE2
#define TFT_HS_BACKGROUND_COLOR TFT_CASE_BLUE//0x33FF
#define TFT_HS_BUTTON_COLOR TFT_CASE_ORANGE//0x76EA
#define GAME_BACKGROUND_COLOR TFT_CASE_BLUE
#define GAME_FOREGROUND_COLOR TFT_CASE_ORANGE
#endif

// ======================================

// Timing Constants ---------------------------
#define VB_TWO 62500
#define VB_ONE 31250
#define VB_THREE_FOURTHS 23438
#define VB_HALF 15625
#define VB_THREE_EIGHTHS 11719
#define VB_FOURTH 7813
#define VB_EIGHTH 3906
#define VB_SIXTEENTH 1953
// --------------------------------------------

// Mazes Constants /////////////////////
#define MAZES_WIDTH 400
#define MAZES_HEIGHT 240
#define MAZES_STARTX 20
#define MAZES_STARTY 16

#define MAZEPATH_BEGINX 187
#define MAZEPATH_BEGINY 177
#define MAZEPATH_FINISHX 171
#define MAZEPATH_FINISHY 9
#define MAZEPATH_WIDTH 360
#define MAZEPATH_HEIGHT 178

///////////////////////////////////////
#endif		// EAPagerX1_Constants_H



#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

void playSnakeGame(void);
void makeNewFood(void);
unsigned int getRandCoord(unsigned int, unsigned char);
void exitSnakeGame(void);

#define UP 1
#define RIGHT 2
#define DOWN -1
#define LEFT -2
#define SPEED_NORMAL 15625
#define X_COORD_TYPE 0
#define Y_COORD_TYPE 1
#define MIN_SNAKE_X_COORD 20
#define MIN_SNAKE_Y_COORD 20
#define MAX_SNAKE_X_COORD (FULL_IMAGE_WIDTH-MIN_SNAKE_X_COORD)
#define MAX_SNAKE_Y_COORD (FULL_IMAGE_HEIGHT-MIN_SNAKE_Y_COORD)
#define MAX_SNAKE_X_COORD_DIV5 (SNAKE_OOB_MAX_X/5)
#define MAX_SNAKE_Y_COORD_DIV5 (SNAKE_OOB_MAX_Y/5)

// Board Boundary Constants //
#define SNAKE_OOB_MIN_X 20
#define SNAKE_OOB_MAX_X 375
#define SNAKE_OOB_MIN_Y 10
#define SNAKE_OOB_MAX_Y 195
//////////////////////////////

#define SNAKE_LEFTBUTTON_BOUNDARY (FULL_IMAGE_WIDTH/4)
#define SNAKE_RIGHTBUTTON_BOUNDARY ((FULL_IMAGE_WIDTH*3)/4)
#define SNAKE_LEFTRIGHTBUTTON_BOUNDARY (FULL_IMAGE_WIDTH/2)
#define SNAKE_UPDOWNBUTTON_BOUNDARY (FULL_IMAGE_HEIGHT/2)


typedef struct snakeNode snakeNode;
struct snakeNode {
	volatile unsigned int x;
	volatile unsigned int y;
	volatile signed char dir;
	volatile signed char oldDir;
	volatile snakeNode *nextSnakeNode;
};

typedef struct foodPiece foodPiece;
struct foodPiece {
	unsigned int x;
	unsigned int y;
};

volatile snakeNode *snakeHead;
volatile snakeNode *snakeTail;
volatile snakeNode *nextSnakeBend;
volatile snakeNode *newestSnakeBend;
volatile snakeNode *tempSnakeNode;

foodPiece food;

volatile unsigned char* snakeGameScore;
unsigned int SGStartX;
unsigned int SGStartY;
unsigned int snakeGameSpeed;
unsigned char SGBlockSize;
unsigned int SGBackgroundColor;
unsigned int snakeColor;
unsigned int foodColor;
unsigned int tailXOffset;
unsigned int foodSeed;
volatile unsigned int snakeSegEnd;
volatile unsigned char inSnakeIntro;


#endif //SNAKE_GAME_H_


#ifndef MAZEGAME_H_
#define MAZEGAME_H_

void playMazeGame(void);
void exitMazeGame(void);
void moveThroughMaze(volatile unsigned int, volatile unsigned int);

#define MAZE_LEFTBUTTON_BOUNDARY (FULL_IMAGE_WIDTH/4)
#define MAZE_RIGHTBUTON_BOUNDARY ((FULL_IMAGE_WIDTH*3)/4)
#define MAZE_UPDOWNBUTTON_BOUNDARY (FULL_IMAGE_HEIGHT/2)

typedef struct mazeNode mazeNode;
struct mazeNode {
	volatile unsigned int x;
	volatile unsigned int y;
	volatile signed char backDir;
	//volatile signed char oldDir;
	volatile mazeNode *prevMazeNode;
};

volatile mazeNode *mazeHead;
volatile mazeNode *mazeTail;
volatile mazeNode *nextMazeBend;
volatile mazeNode *newestMazeBend;
volatile mazeNode *tempMazeNode;

unsigned int MGStartX;
unsigned int MGStartY;
unsigned char MGBlockSize;
unsigned int MGBackgroundColor;
unsigned int userPathColor;
volatile unsigned int mazeSegEnd;
unsigned char currentMaze;
volatile unsigned char mazeSolved;
unsigned char currentMazeImageId;
unsigned char currentMazePathId;
unsigned int mazePrintOffsetX;
unsigned int mazePrintOffsetY;
volatile unsigned int segFixedCoord; // Also used in snakeGame
volatile unsigned char inMazeIntro;

 

#endif /* MAZEGAME_H_ */