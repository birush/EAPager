#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "EAPagerX1.h"
#include "EAImages.h"
#include <util/delay.h>


void playMazeGame() {
	// Initialize some game parameters
	
	switch(currentMaze) {
		case 0:
			currentMazeImageId = MAZE0_IMAGE_ID;
			currentMazePathId = MAZE0_PATH_ID;
			break;
		case 1:
			currentMazeImageId = MAZE1_IMAGE_ID;
			currentMazePathId = MAZE1_PATH_ID;
			break;
		case 2:
			currentMazeImageId = MAZE2_IMAGE_ID;
			currentMazePathId = MAZE2_PATH_ID;
			break;
		case 3:
			currentMazeImageId = MAZE3_IMAGE_ID;
			currentMazePathId = MAZE3_PATH_ID;
			break;
		case 4:
			currentMazeImageId = MAZE4_IMAGE_ID;
			currentMazePathId = MAZE4_PATH_ID;
			break;
	}
	
	mazeSolved = 0;	
	//MGStartX;
	//MGStartY;
	MGBlockSize = 4;
	mazePrintOffsetX = MAZES_STARTX + 2 - MGBlockSize;
	mazePrintOffsetY = MAZES_STARTY + 2 - MGBlockSize;
	MGBackgroundColor = GAME_BACKGROUND_COLOR;
	userPathColor = GAME_FOREGROUND_COLOR;
	
	// Initialize tail
	mazeTail = (volatile mazeNode*)malloc(sizeof(mazeNode));
	mazeTail->x = MAZEPATH_BEGINX;
	mazeTail->y = MAZEPATH_BEGINY;
	mazeTail->backDir = 0;
	mazeTail->prevMazeNode = NULL;
	
	// Initialize head
	mazeHead = (volatile mazeNode*)malloc(sizeof(mazeNode));
	mazeHead->x = MAZEPATH_BEGINX;
	mazeHead->y = MAZEPATH_BEGINY;
	mazeHead->backDir = 0;
	mazeHead->prevMazeNode = mazeTail;

	// Print Background Stuff to Screen
	tft_print_image(currentMazeImageId, MGBackgroundColor, TFT_BLACK, 0, 0);
	printDigits(SGBackgroundColor, PILDigits, PIL_STARTX, PIL_STARTY);
	tft_print_image(MAIN_MENU_BUTTON_ID, TFT_GREEN, TFT_BLACK, MAIN_MENU_BUTTON_STARTX, MAIN_MENU_BUTTON_STARTY);
	
	// Print User Trail to Screen
	tft_print_square(MAZEPATH_BEGINX+mazePrintOffsetX, MAZEPATH_BEGINY+mazePrintOffsetY, userPathColor, MGBlockSize);
	
	changingProgram = 0;
	touchSenseReset();
	
	// Begin Game
	while (!changingProgram && !mazeSolved) {
		if (PILChanged) {
			printDigits(SGBackgroundColor, PILDigits, PIL_STARTX, PIL_STARTY);
			PILChanged = 0;
		}
	}
	
	exitMazeGame();
}

void exitMazeGame() {
	// Disable Touch Sensing
	PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
	PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
	
	// Print Win Screen and score //////////
	
	////////////////////////////////////////
	
	// Free Node memory ////////////////////
	while (mazeHead->prevMazeNode != NULL) {
		tempMazeNode = mazeHead->prevMazeNode;
		free((mazeNode*)mazeHead);
		mazeHead = tempMazeNode;
	}
	free((mazeNode*)mazeHead);		// Free last Node
	////////////////////////////////////////
	
	if (mazeSolved) currentMaze++;	// Send user to next maze next time they play
	currentProgram = 0;			// Go to Main Menu
	
	/*	
	for (int i=0; i < 2500; i++) {	// Give user time to see the Win screen
		_delay_ms(1);
	}
	*/
}

void moveThroughMaze(volatile unsigned int xCoord, volatile unsigned int yCoord) {
	unsigned int newXCoord;
	unsigned int newYCoord;
	signed char directionOfMove;

	if(xCoord < MAZE_LEFTBUTTON_BOUNDARY) {				// Moving Left
		 newXCoord = mazeHead->x - MGBlockSize;
		 newYCoord = mazeHead->y;
		 directionOfMove = LEFT;
	}
	
	else if(xCoord > MAZE_RIGHTBUTON_BOUNDARY) {		// Moving Right
		newXCoord = mazeHead->x + MGBlockSize;
		newYCoord = mazeHead->y;
		directionOfMove = RIGHT;
	}
	
	else if (yCoord < MAZE_UPDOWNBUTTON_BOUNDARY) {		// Moving Up
		newXCoord = mazeHead->x;
		newYCoord = mazeHead->y - MGBlockSize;
		directionOfMove = UP;
	}
	
	else {												// Moving Down
		newXCoord = mazeHead->x;
		newYCoord = mazeHead->y + MGBlockSize;
		directionOfMove = DOWN;
	}
	
	if (getPixel(currentMazePathId, newXCoord, newYCoord) == 0) {	// This was a valid direction to move from this location
		if (mazeHead->backDir == directionOfMove) {						// Moving Backwards
			if (newXCoord == mazeHead->prevMazeNode->x && newYCoord == mazeHead->prevMazeNode->y) {	// We're at the last bend node: free maze head memory, make head = prevMazeNode
				// Update Screen to reflect movement
				tft_print_square(mazeHead->x+mazePrintOffsetX, mazeHead->y+mazePrintOffsetY, MGBackgroundColor, MGBlockSize); // "Erasing furthest block"
				
				tempMazeNode = mazeHead->prevMazeNode;
				free((mazeNode*)mazeHead);
				mazeHead = tempMazeNode;
			}
			else {		// Not reaching the last bend node: just erase current head then increment head back
				// Update Screen to reflect movement
				tft_print_square(mazeHead->x+mazePrintOffsetX, mazeHead->y+mazePrintOffsetY, MGBackgroundColor, MGBlockSize); // "Erasing furthest block"
				
				mazeHead->x = newXCoord;
				mazeHead->y = newYCoord;
			}
		}
		
		else {															// Moving Forwards
			if (mazeHead->backDir == -directionOfMove) {				// Moving in the same direction as before: move head but don't create new bend node
				mazeHead->x = newXCoord;
				mazeHead->y = newYCoord;
			}
					
					
					
			else {														// Changing directions: move head, create new bend node
				tempMazeNode = mazeHead;	// create new node pointer
				mazeHead = (volatile mazeNode*)malloc(sizeof(mazeNode));
				mazeHead->backDir = -directionOfMove;
				mazeHead->x = newXCoord;
				mazeHead->y = newYCoord;
				mazeHead->prevMazeNode = tempMazeNode;
			}	
			
			// Update Screen to reflect movement
			tft_print_square(mazeHead->x+mazePrintOffsetX, mazeHead->y+mazePrintOffsetY, userPathColor, MGBlockSize);
			
			if (mazeHead->x == MAZEPATH_FINISHX && mazeHead->y == MAZEPATH_FINISHY) {		// Finished!
				mazeSolved = 1;
			}
		}
	}	
}



