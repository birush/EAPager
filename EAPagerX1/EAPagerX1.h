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


#endif

#ifndef HS_BACKGROUND_IMAGE
#define HS_BACKGROUND_IMAGE 255
#endif

#ifndef HS_MAZES_BUTTON_IMAGE
#define HS_MAZES_BUTTON_IMAGE 254
#endif

#ifndef HS_SNAKE_BUTTON_IMAGE
#define HS_SNAKE_BUTTON_IMAGE 253
#endif

#ifndef MAZE0_IMAGE
#define MAZE0_IMAGE 10
#endif

#ifndef MAZE1_IMAGE
#define MAZE1_IMAGE 11
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

#ifndef NUM0
#define NUM0 0
#endif

#ifndef NUM1
#define NUM1 1
#endif

#ifndef NUM2
#define NUM2 2
#endif

#ifndef NUM3
#define NUM3 3
#endif

#ifndef NUM4
#define NUM4 4
#endif

#ifndef NUM5
#define NUM5 5
#endif

#ifndef NUM6
#define NUM6 6
#endif

#ifndef NUM7
#define NUM7 7
#endif

#ifndef NUM8
#define NUM8 8
#endif

#ifndef NUM9
#define NUM9 9
#endif

