#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "EAPagerX1.h"
#include "EAImages.h"
//#include "SnakeGame.h"



void playSnakeGame() {
	// Initialize Some Game Parameters
	startX = 100;
	startY = 120;
	gameSpeed = VB_FOURTH;
	blockSize = 5;
	foodSeed = 100;

	// Initialize Game Timer
	PMIC_CTRL |= 0x01;	// Enable Low level interrupts in PMIC
	TCE0_CTRLB = 0x10;		// Normal counting mode, Enable CCA Only
	TCE0_INTCTRLB = 0x01;	// CCA interrupt enabled at low priority
	TCE0_PER = gameSpeed;	// Set Top
	TCE0_CCA = gameSpeed;	// Set CCA	// Interrupt will occur at (gameSpeed * 32) us
	
	// Initialize head
	//snakeNode firstHead;
	head = (volatile snakeNode*)malloc(sizeof(snakeNode));
	head->x = startX;
	head->y = startY;
	head->dir = RIGHT;
	head->oldDir = RIGHT;
	head->nextSnakeNode = NULL;
	
	// Initialize tail
	//snakeNode firstTail;
	tail = (volatile snakeNode*)malloc(sizeof(snakeNode));
	tail->x = startX-(4*blockSize);
	tail->y = startY;
	tail->dir = RIGHT;
	tail->oldDir = RIGHT;
	tail->nextSnakeNode = head;
	
	// Initialize colors
	backGroundColor = TFT_CASE_BLUE;
	snakeColor = TFT_ORANGE;
	foodColor = TFT_GREEN;
	
	// Print background
	
	tft_print_blank_background(backGroundColor);
	tft_print_image(MAIN_MENU_BUTTON_ID, TFT_HS_BUTTON_COLOR, TFT_BLACK, MAIN_MENU_BUTTON_STARTX, MAIN_MENU_BUTTON_STARTY);
	
	// Initialize and print food
	makeNewFood();
	
	// Print snake
	int i;
	for(i = tail->x; i <= head->x; i += blockSize) {
		tft_print_square(i, head->y, snakeColor, blockSize);
	}
	
	changingProgram = 0;
	touchSenseReset();
	
	// Start Game
	TCE0_CTRLA = 0x07;	// Start game timer at 31.25 kHz
	
	while (!changingProgram) {
		//debug=6;
	}
	
	// Exit Game Release memory, clean up
	while (tail->nextSnakeNode != NULL){
		tempSnakeNode = tail->nextSnakeNode;
		free((snakeNode*)tail);
		tail = tempSnakeNode;
	}
	free((snakeNode*)tail);								// Free last node
}

// On Game Timer Interrupt
ISR(TCE0_CCA_vect) {
	if (head->dir != head->oldDir) {		//Snake turning, need to create new bend node
		
		
		tempSnakeNode = head;	// create new node pointer
		//snakeNode newNode;
		head = (volatile snakeNode*)malloc(sizeof(snakeNode));	// Create new node for head
		head->dir = tempSnakeNode->dir;
		head->oldDir = tempSnakeNode->dir;
		head->x = tempSnakeNode->x;
		head->y = tempSnakeNode->y;
		head->nextSnakeNode = 0;
		tempSnakeNode->nextSnakeNode = head;
		newestBend = tempSnakeNode;
	}
	
	switch(head->dir) {
		case UP:
			head->y-= blockSize;
			break; 
		case RIGHT:
			head->x+= blockSize;
			break;
		case DOWN:
			head->y+= blockSize;
			break;
		case LEFT:
			head->x-= blockSize;
			break;
	}
	
	if (head->x == food.x && head->y == food.y) {
		tft_print_square(head->x, head->y, snakeColor, blockSize);		// Print new head block
		makeNewFood();
	}
	else {
		tft_print_square(head->x, head->y, snakeColor, blockSize);		// Print new head block
		tft_print_square(tail->x, tail->y, backGroundColor, blockSize);	// Erase tail block
		switch(tail->dir) {												// Increment tail
			case UP:
			tail->y -= blockSize;
			break;
			case RIGHT:
			tail->x += blockSize;
			break;
			case DOWN:
			tail->y += blockSize;
			break;
			case LEFT:
			tail->x -= blockSize;
			break;
		}
		
		if (tail->x == tail->nextSnakeNode->x && tail->y == tail->nextSnakeNode->y) {	// If tail has reached a bend, free tail memory space
			tempSnakeNode = tail->nextSnakeNode;
			free((snakeNode*)tail);
			tail = tempSnakeNode;
		}
	}
	
}

void makeNewFood() {	// Change food position, print new food block
	foodSeed = foodSeed + 131;
	food.x = getRandCoord(foodSeed, X_COORD_TYPE);
	food.y = getRandCoord(foodSeed, Y_COORD_TYPE);
	tft_print_square(food.x, food.y, foodColor, blockSize);
}

unsigned int getRandCoord(unsigned int seed, unsigned char coordType) {
	unsigned long Xn = seed;
	for (int i=0; i < 5; i++) {
		Xn = (16807*Xn) % 2147483647;
	}
	//Xn = Xn*10
	
	if (coordType) {
		return Xn % MAX_SNAKE_Y_COORD;
	}
	else {
		return Xn % MAX_SNAKE_X_COORD;
	}
}


