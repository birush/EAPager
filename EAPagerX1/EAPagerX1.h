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

#endif

//======= IMAGE CONSTANTS ========================
#ifndef HS_BACKGROUND_IMAGE_ID
#define HS_BACKGROUND_IMAGE_ID 255
#endif

#ifndef FULL_IMAGE_WIDTH
#define FULL_IMAGE_WIDTH 400
#endif

#ifndef FULL_IMAGE_HEIGHT
#define FULL_IMAGE_HEIGHT 240
#endif

// Mazes Button //////////////////////
#ifndef HS_MAZES_BUTTON_IMAGE_ID
#define HS_MAZES_BUTTON_IMAGE_ID 254
#endif

#ifndef HS_MAZES_BUTTON_STARTX
#define HS_MAZES_BUTTON_STARTX 48
#endif

#ifndef HS_MAZES_BUTTON_STARTY
#define HS_MAZES_BUTTON_STARTY 68
#endif
/////////////////////////////////////

// Snake Button /////////////////////
#ifndef HS_SNAKE_BUTTON_IMAGE_ID
#define HS_SNAKE_BUTTON_IMAGE_ID 253
#endif

#ifndef HS_SNAKE_BUTTON_STARTX
#define HS_SNAKE_BUTTON_STARTX 248
#endif

#ifndef HS_SNAKE_BUTTON_STARTY
#define HS_SNAKE_BUTTON_STARTY 68
#endif
/////////////////////////////////////

#ifndef HS_BUTTON_IMAGE_WIDTH
#define HS_BUTTON_IMAGE_WIDTH 104
#endif

#ifndef HS_BUTTON_IMAGE_HEIGHT
#define HS_BUTTON_IMAGE_HEIGHT 104
#endif

// Main Menu Button //////////////////
#ifndef MAIN_MENU_BUTTON_ID
#define MAIN_MENU_BUTTON_ID 252
#endif

#ifndef MAIN_MENU_BUTTON_WIDTH
#define MAIN_MENU_BUTTON_WIDTH 72
#endif

#ifndef MAIN_MENU_BUTTON_HEIGHT
#define MAIN_MENU_BUTTON_HEIGHT 16
#endif

#ifndef MAIN_MENU_BUTTON_STARTX
#define MAIN_MENU_BUTTON_STARTX 325
#endif

#ifndef MAIN_MENU_BUTTON_STARTY
#define MAIN_MENU_BUTTON_STARTY 220
#endif
///////////////////////////////////////

#ifndef MAZE0_IMAGE_ID
#define MAZE0_IMAGE_ID 10
#endif

#ifndef MAZE1_IMAGE_ID
#define MAZE1_IMAGE_ID 11
#endif

#ifndef PIL_STARTX
#define PIL_STARTX 245
#endif

#ifndef PIL_STARTY
#define PIL_STARTY 210
#endif

#ifndef PIL_SPACING
#define PIL_SPACING 14
#endif

#ifndef NUM0_ID
#define NUM0_ID 0
#endif

#ifndef NUM1_ID
#define NUM1_ID 1
#endif

#ifndef NUM2_ID
#define NUM2_ID 2
#endif

#ifndef NUM3_ID
#define NUM3_ID 3
#endif

#ifndef NUM4_ID
#define NUM4_ID 4
#endif

#ifndef NUM5_ID
#define NUM5_ID 5
#endif

#ifndef NUM6_ID
#define NUM6_ID 6
#endif

#ifndef NUM7_ID
#define NUM7_ID 7
#endif

#ifndef NUM8_ID
#define NUM8_ID 8
#endif

#ifndef NUM9_ID
#define NUM9_ID 9
#endif

#ifndef NUM_WIDTH
#define NUM_WIDTH 16
#endif

#ifndef NUM_HEIGHT
#define NUM_HEIGHT 16
#endif


// =================================

//===== Game Constants ============
#ifndef MAIN_MENU_ID
#define MAIN_MENU_ID 0
#endif

#ifndef MAZE_GAME_ID
#define MAZE_GAME_ID 1
#endif

#ifndef SNAKE_GAME_ID
#define SNAKE_GAME_ID 2
#endif