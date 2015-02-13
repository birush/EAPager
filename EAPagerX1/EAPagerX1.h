#ifndef EAPagerX1_HEADER
#define EAPagerX1_HEADER



void tft_init();
void tft_write_command(char);
void tft_write_data16(unsigned int);
void tft_write_data8(char);
void tft_print_image(unsigned char*);
void setWindow(unsigned int, unsigned int, unsigned int, unsigned int);
void touchInit();
void touchSenseInit();
void measureTouchCoordinates();
void adc_enable();
void adc_take_sample(char);
int playMazeGame(void);


#endif