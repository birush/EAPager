#ifndef EAPagerX1_H
#define EAPagerX1_H


void clock_init(void);
void tft_init(void);
void tft_write_command(char);
void tft_write_data16(unsigned int);
void tft_write_data8(char);
void setWindow(unsigned int, unsigned int, unsigned int, unsigned int);
void tft_print_image(unsigned char, unsigned int);
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