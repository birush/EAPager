#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "EAPagerX1.h"
#include "EAImages.h"
#include <util/delay.h>
//#include "SnakeGame.h"



void playSnakeGame() {
	// Initialize Some Game Parameters
	SGStartX = 100;
	SGStartY = 120;
	snakeGameSpeed = VB_EIGHTH;
	SGBlockSize = 5;
	foodSeed = 100;
	*snakeGameScore = 0;

	// Initialize Game Timer
	PMIC_CTRL |= 0x01;	// Enable Low level interrupts in PMIC
	TCE0_CTRLB = 0x10;		// Normal counting mode, Enable CCA Only
	TCE0_INTCTRLB = 0x01;	// CCA interrupt enabled at low priority
	TCE0_PER = snakeGameSpeed;	// Set Top
	TCE0_CCA = snakeGameSpeed;	// Set CCA	// Interrupt will occur at (gameSpeed * 32) us
	
	// Start timer for seeding random number generator
	TCD0_CTRLB = 0x00;		// Normal counting mode, no CCs enabled
	TCD0_PER = 0xFFFF;		// Set period as high as possible
	TCD0_CTRLA = 0x07;		// Start random number seed timer
	
	// Initialize head
	//snakeNode firstHead;
	snakeHead = (volatile snakeNode*)malloc(sizeof(snakeNode));
	snakeHead->x = SGStartX;
	snakeHead->y = SGStartY;
	snakeHead->dir = RIGHT;
	snakeHead->oldDir = RIGHT;
	snakeHead->nextSnakeNode = NULL;
	
	// Initialize tail
	//snakeNode firstTail;
	snakeTail = (volatile snakeNode*)malloc(sizeof(snakeNode));
	snakeTail->x = SGStartX-(4*SGBlockSize);
	snakeTail->y = SGStartY;
	snakeTail->dir = RIGHT;
	snakeTail->oldDir = RIGHT;
	snakeTail->nextSnakeNode = snakeHead;
	
	// Initialize colors
	SGBackgroundColor = GAME_BACKGROUND_COLOR;
	snakeColor = GAME_FOREGROUND_COLOR;
	foodColor = TFT_GREEN;
	
	// Print background
	tft_print_image(SG_BACKGROUND_IMAGE_ID, SGBackgroundColor, TFT_BLACK, 0, 0);
	printDigits(SGBackgroundColor, PILDigits, PIL_STARTX, PIL_STARTY);
	tft_print_image(MAIN_MENU_BUTTON_ID, TFT_GREEN, TFT_BLACK, MAIN_MENU_BUTTON_STARTX, MAIN_MENU_BUTTON_STARTY);
	
	
	// Initialize and print food
	makeNewFood();
	
	// Print snake
	int i;
	for(i = snakeTail->x; i <= snakeHead->x; i += SGBlockSize) {
		tft_print_square(i, snakeHead->y, snakeColor, SGBlockSize);
	}
	
	changingProgram = 0;
	touchSenseReset();
	
	// Start Game
	TCE0_CTRLA = 0x07;	// Start game timer at 31.25 kHz
	
	while (!changingProgram) {
		if (PILChanged) {
			printDigits(SGBackgroundColor, PILDigits, PIL_STARTX, PIL_STARTY);
			PILChanged = 0;
		}
	}
	
	exitSnakeGame();
	
}

void exitSnakeGame() {							// Release memory, clean up, go back to main menu
	// Disable Touch Sensing
	PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
	PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
	
	// Stop and Disable Game Timer
	TCE0_CTRLA = 0x00;	// Stop game timer
	TCE0_CTRLB = 0x00;		// Disable CCA
	TCE0_INTCTRLB = 0x00;	// Disable TCE0 interrupts
	
	// Print "Game Over" and score //////////
	tft_print_image(SNAKE_GAME_OVER_ID, TFT_ORANGE, TFT_BLACK, SNAKE_GAME_OVER_STARTX, SNAKE_GAME_OVER_STARTY);
	calculateDigits(snakeGameScore, SGSDigits);
	printDigits(TFT_ORANGE, SGSDigits, SGS_STARTX, SGS_STARTY);
	/////////////////////////////////////////
		
	while (snakeTail->nextSnakeNode != NULL){
		tempSnakeNode = snakeTail->nextSnakeNode;
		free((snakeNode*)snakeTail);
		snakeTail = tempSnakeNode;
	}
	free((snakeNode*)snakeTail);		// Free last node
	
	currentProgram = 0;			// Go to Main Menu
	
	for (int i=0; i < 2500; i++) {	// Give user time to see their score
		_delay_ms(1);
	}
}

// On Game Timer Interrupt
ISR(TCE0_CCA_vect) {
	if (snakeHead->dir != snakeHead->oldDir) {		//Snake turning, need to create new bend node
		
		
		tempSnakeNode = snakeHead;	// create new node pointer
		//snakeNode newNode;
		snakeHead = (volatile snakeNode*)malloc(sizeof(snakeNode));	// Create new node for head
		snakeHead->dir = tempSnakeNode->dir;
		snakeHead->oldDir = tempSnakeNode->dir;
		snakeHead->x = tempSnakeNode->x;
		snakeHead->y = tempSnakeNode->y;
		snakeHead->nextSnakeNode = 0;
		tempSnakeNode->nextSnakeNode = snakeHead;
		newestSnakeBend = tempSnakeNode;
	}
	
	switch(snakeHead->dir) {
		case UP:
			snakeHead->y-= SGBlockSize;
			break; 
		case RIGHT:
			snakeHead->x+= SGBlockSize;
			break;
		case DOWN:
			snakeHead->y+= SGBlockSize;
			break;
		case LEFT:
			snakeHead->x-= SGBlockSize;
			break;
	}
	
	// Check for collision with self /////////////////////////////////////////////////////////////////////
	tempSnakeNode = snakeTail;
	while (tempSnakeNode->nextSnakeNode != NULL){
		switch(tempSnakeNode->dir) {
			case UP:
				if (snakeHead->x == tempSnakeNode->x) {
					snakeSegEnd = tempSnakeNode->nextSnakeNode->y;
					if (tempSnakeNode->nextSnakeNode == snakeHead) snakeSegEnd += SGBlockSize;	// Head being in same place as head isn't a collision
					
					if (snakeHead->y <= tempSnakeNode->y && snakeHead->y >= snakeSegEnd) {
						changingProgram = 1;
						return;
					}
				}
				break;
			case RIGHT:
				snakeSegEnd = tempSnakeNode->nextSnakeNode->x;
				if (tempSnakeNode->nextSnakeNode == snakeHead) snakeSegEnd -= SGBlockSize;	// Head being in same place as head isn't a collision
				
				if (snakeHead->y == tempSnakeNode->y) {
					if (snakeHead->x >= tempSnakeNode->x && snakeHead->x <= snakeSegEnd) {
						changingProgram = 1;
						return;
					}
				}
				break;
			case DOWN:
				snakeSegEnd = tempSnakeNode->nextSnakeNode->y;
				if (tempSnakeNode->nextSnakeNode == snakeHead) snakeSegEnd -= SGBlockSize;	// Head being in same place as head isn't a collision
				
				if (snakeHead->x == tempSnakeNode->x) {
					if (snakeHead->y >= tempSnakeNode->y && snakeHead->y <= snakeSegEnd) {
						changingProgram = 1;
						return;
					}
				}
				break;
			case LEFT:
				snakeSegEnd = tempSnakeNode->nextSnakeNode->x;
				if (tempSnakeNode->nextSnakeNode == snakeHead) snakeSegEnd += SGBlockSize;	// Head being in same place as head isn't a collision
			
				if (snakeHead->y == tempSnakeNode->y) {
					if (snakeHead->x <= tempSnakeNode->x && snakeHead->x >= snakeSegEnd) {
						changingProgram = 1;
						return;
					}
				}
				break;
		}
		
		tempSnakeNode = tempSnakeNode->nextSnakeNode;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if (snakeHead->x > SNAKE_OOB_MAX_X || snakeHead->x < SNAKE_OOB_MIN_X || snakeHead->y > SNAKE_OOB_MAX_Y || snakeHead->y < SNAKE_OOB_MIN_Y){	// Check for Out of Bounds
		changingProgram = 1;
		return;
	}
	
	else if (snakeHead->x == food.x && snakeHead->y == food.y) {					// Reached Food?
		tft_print_square(snakeHead->x, snakeHead->y, snakeColor, SGBlockSize);		// Print new head block
		makeNewFood();
		(*snakeGameScore)++;
	}
	
	else {
		tft_print_square(snakeHead->x, snakeHead->y, snakeColor, SGBlockSize);		// Print new head block
		tft_print_square(snakeTail->x, snakeTail->y, SGBackgroundColor, SGBlockSize);	// Erase tail block
		switch(snakeTail->dir) {												// Increment tail
			case UP:
			snakeTail->y -= SGBlockSize;
			break;
			case RIGHT:
			snakeTail->x += SGBlockSize;
			break;
			case DOWN:
			snakeTail->y += SGBlockSize;
			break;
			case LEFT:
			snakeTail->x -= SGBlockSize;
			break;
		}
		
		if (snakeTail->x == snakeTail->nextSnakeNode->x && snakeTail->y == snakeTail->nextSnakeNode->y) {	// If tail has reached a bend, free tail memory space
			tempSnakeNode = snakeTail->nextSnakeNode;
			free((snakeNode*)snakeTail);
			snakeTail = tempSnakeNode;
		}
	}
	
	// Check for Collisions /////////////////////////////
	
	/////////////////////////////////////////////////////
}

void makeNewFood() {	// Change food position, print new food block
	foodSeed = TCD0_CNT;
	food.x = getRandCoord(foodSeed, X_COORD_TYPE);
	food.y = getRandCoord(foodSeed, Y_COORD_TYPE);
	tft_print_square(food.x, food.y, foodColor, SGBlockSize);
}

unsigned int getRandCoord(unsigned int seed, unsigned char coordType) {
	unsigned long Xn = seed;
	for (int i=0; i < 2; i++) {
		Xn = (16807*Xn) % 2147483647;
	}
	
	if (coordType) {					// Y coordinate
		Xn %= MAX_SNAKE_Y_COORD_DIV5;
		Xn *= 5;
		if (Xn < SNAKE_OOB_MIN_Y) Xn = SNAKE_OOB_MIN_Y;
	}
	else {								// X coordinate
		Xn %= MAX_SNAKE_X_COORD_DIV5;
		Xn *= 5;
		if (Xn < SNAKE_OOB_MIN_X) Xn = SNAKE_OOB_MIN_X;
	}
	
	return (unsigned int)Xn;
}


