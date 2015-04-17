#ifndef IMAGES
#define IMAGES

unsigned char getPixel(unsigned char, unsigned int, unsigned int);

extern const __memx unsigned char homeScreenBackground[240][50];
extern const __memx unsigned char mazesButton[104][13];
extern const __memx unsigned char snakeButton[104][13];
extern const __memx unsigned char mainMenuButton[16][9];
extern const __memx unsigned char snakeGameBackground[240][50];
extern const __memx unsigned char snakeGameOver[140][37];

extern const __memx unsigned char okButton[30][11];
extern const __memx unsigned char tableReadyImage[50][50];
extern const __memx unsigned char pagingUserImage[240][50];
extern const __memx unsigned char outOfRangeImage[240][50];
extern const __memx unsigned char mazeSolvedImage[140][37];
extern const __memx unsigned char mazeGameIntroImage[240][50];
extern const __memx unsigned char snakeGameIntroImage[240][50];

extern const __memx unsigned char maze0[240][50];
extern const __memx unsigned char maze0Path[178][45];
extern const __memx unsigned char maze1[240][50];
extern const __memx unsigned char maze1Path[178][45];
extern const __memx unsigned char maze2[240][50];
extern const __memx unsigned char maze2Path[178][45];
extern const __memx unsigned char maze3[240][50];
extern const __memx unsigned char maze3Path[178][45];
extern const __memx unsigned char maze4[240][50];
extern const __memx unsigned char maze4Path[178][45];


extern const __memx unsigned char num0[16][2];
extern const __memx unsigned char num1[16][2];
extern const __memx unsigned char num2[16][2];
extern const __memx unsigned char num3[16][2];
extern const __memx unsigned char num4[16][2];
extern const __memx unsigned char num5[16][2];
extern const __memx unsigned char num6[16][2];
extern const __memx unsigned char num7[16][2];
extern const __memx unsigned char num8[16][2];
extern const __memx unsigned char num9[16][2];

// efficientImagePrint =============================================================

extern const __memx unsigned char altNum0[16][2];
//extern const __memx unsigned char* num0ptr = &num0[0][0];

typedef struct savedImage savedImage;
struct savedImage {
	unsigned char arrayWidth;
	unsigned char width;
	unsigned char height;
	const __memx unsigned char* data;
};

//==================================================================================

#endif