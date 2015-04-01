#include <avr/io.h>
#include <avr/interrupt.h>
#include "EAPagerX1.h"
#include "EAImages.h"
#include "SnakeGame.h"

#define SPEED_NORMAL 15625

typedef struct snakeNode snakeNode;
struct snakeNode {
	unsigned int x;
	unsigned int y;
	unsigned char dir;
	snakeNode *nextSnakeNode;
};

typedef struct foodPiece foodPiece;
struct foodPiece {
	unsigned int x;
	unsigned int y;
};

snakeNode *head;
snakeNode *tail;
snakeNode *nextBend;
snakeNode *lastBend;

foodPiece food;

unsigned int startX = 100;
unsigned int startY = 120;
unsigned int gameSpeed = SPEED_NORMAL;
unsigned char blockSize;
unsigned int backGroundColor;
unsigned int snakeColor;
unsigned int foodColor;



void playSnakeGame() {
	// Initialize Game Timer
	PMIC_CTRL |= 0x01;	// Enable Low level interrupts in PMIC
	TCE0_CTRLB = 0x10;		// Normal counting mode, Enable CCA Only
	TCE0_INTCTRLB = 0x01;	// CCA interrupt enabled at low priority
	TCE0_PER = gameSpeed;	// Set Top
	TCE0_CCA = gameSpeed;	// Set CCA	// Interrupt will occur at (gameSpeed * 32) us
	//TCE0_CTRLA = 0x07;	// Start game timer at 31.25 kHz
	
	// Initialize head
	snakeNode firstHead;
	head = &firstHead;
	head->x = startX;
	head->y = startY;
	head->dir = RIGHT;
	head->nextSnakeNode = 0;
	
	// Initialize tail
	snakeNode firstTail;
	tail = &firstTail;
	tail->x = startX-4;
	tail->y = startY;
	tail->dir = RIGHT;
	tail->nextSnakeNode = head;
	
	// Initialize snake and food size
	blockSize = 10;
	
	// Initialize colors
	backGroundColor = TFT_BLUE;
	snakeColor = TFT_ORANGE;
	foodColor = TFT_GREEN;
	
	// Initialize food
	makeNewFood();
	
	// Exit Game Release memory, clean up
	
}

// On Game Timer Interrupt
ISR(TCE0_CCA_vect) {
	switch(head->dir) {
		case UP:
			head->y++;
			break; 
		case RIGHT:
			head->x++;
			break;
		case DOWN:
			head->y--;
			break;
		case LEFT:
			head->x--;
			break;
	}
	
	if (head->x == food.x && head->y == food.y) {
		makeNewFood();
		tft_print_square(food.x, food.y, foodColor, blockSize);
	}
	else {
		tft_print_square(head->x, head->y, snakeColor, blockSize);		// Print new head block
		tft_print_square(tail->x, tail->y, backGroundColor, blockSize);	// Erase tail block
		switch(tail->dir) {												// Increment tail
			case UP:
			tail->y++;
			break;
			case RIGHT:
			tail->x++;
			break;
			case DOWN:
			tail->y--;
			break;
			case LEFT:
			tail->x--;
			break;
		}
		
		if (tail->x == tail->nextSnakeNode->x && tail->y == tail->nextSnakeNode->y) {
			
		}
	}
	
}

void makeNewFood() {
	food.x = 300;
	food.y = 200;
}
