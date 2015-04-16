#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "EAPagerX1.h"
#include "EAImages.h"
#include <util/delay.h>


void playMazeGame(void)
{
	inMazeIntro = 1;
	mazeSolved = 0;
	//MGStartX;
	//MGStartY;
	MGBlockSize = 8;
	mazePrintOffsetX = MAZES_STARTX /*+ 2 */- (MGBlockSize/2);
	mazePrintOffsetY = MAZES_STARTY /*+ 2 */- (MGBlockSize/2);
	MGBackgroundColor = GAME_BACKGROUND_COLOR;
	userPathColor = GAME_FOREGROUND_COLOR;
	
	// Print Intro Screen
	tft_print_image(MG_INTRO_IMAGE_ID, GAME_BACKGROUND_COLOR, TFT_BLACK, 0, 0);
	tft_print_image(OK_BUTTON_RC_ID, TFT_GREEN, TFT_BLACK, OK_BUTTON_RC_STARTX, OK_BUTTON_RC_STARTY);
	
	changingProgram = 0;
	touchSenseReset();
	
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
	
	// Wait till user is done with intro to start game
	while (inMazeIntro) {
		
	}

	// Print Background Stuff to Screen
	tft_print_image(currentMazeImageId, MGBackgroundColor, TFT_BLACK, 0, 0);
	printDigits(MGBackgroundColor, PILDigits, PIL_STARTX, PIL_STARTY);
	tft_print_image(MAIN_MENU_BUTTON_ID, TFT_GREEN, TFT_BLACK, MAIN_MENU_BUTTON_STARTX, MAIN_MENU_BUTTON_STARTY);
	
	// Print User Trail to Screen
	tft_print_square(MAZEPATH_BEGINX+mazePrintOffsetX, MAZEPATH_BEGINY+mazePrintOffsetY, userPathColor, MGBlockSize);	
	
	// Begin Game
	while (changingProgram == 0 && mazeSolved == 0) {
		if (PILChanged) {
			printDigits(MGBackgroundColor, PILDigits, PIL_STARTX, PIL_STARTY);
			PILChanged = 0;
		}
		
		if (tableReady) {
			// Disable Touch Sensing
			PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
			PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
			
			tableReadyMessage();
		}
		
		if (pagingUser) {
			// Disable Touch Sensing
			PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
			PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
			
			pageUser();
		}
		
		if (outOfRange) {
			// Disable Touch Sensing
			PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
			PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
			
			timeSincePing = 0;
			showOORMessage();
		}
		
		if (resumingProgram) {	// Resuming from suspended state
			currentProgram = MAZE_GAME_ID;
			
			// Reprint background stuff
			tft_print_image(currentMazeImageId, MGBackgroundColor, TFT_BLACK, 0, 0);
			printDigits(MGBackgroundColor, PILDigits, PIL_STARTX, PIL_STARTY);
			tft_print_image(MAIN_MENU_BUTTON_ID, TFT_GREEN, TFT_BLACK, MAIN_MENU_BUTTON_STARTX, MAIN_MENU_BUTTON_STARTY);
			
			// Reprint user trail ////////////////////////////////////////////////////////////////////////////////////////////
			tempMazeNode = mazeHead;
			while (tempMazeNode != mazeTail) {
				switch(tempMazeNode->backDir) {		// Print every square in this user trail segment
					case UP:
					segFixedCoord = tempMazeNode->x;	// x coord doesn't change on this segment
					for (int i=tempMazeNode->y; i > tempMazeNode->prevMazeNode->y; i -= MGBlockSize) {
						tft_print_square(segFixedCoord+mazePrintOffsetX, i+mazePrintOffsetY, userPathColor, MGBlockSize);
					}
					break;
					case DOWN:
					segFixedCoord = tempMazeNode->x;	// x coord doesn't change on this segment
					for (int i=tempMazeNode->y; i < tempMazeNode->prevMazeNode->y; i += MGBlockSize) {
						tft_print_square(segFixedCoord+mazePrintOffsetX, i+mazePrintOffsetY, userPathColor, MGBlockSize);
					}
					break;
					case RIGHT:
					segFixedCoord = tempMazeNode->y;	// y coord doesn't change on this segment
					for (int i=tempMazeNode->x; i < tempMazeNode->prevMazeNode->x; i += MGBlockSize) {
						tft_print_square(i+mazePrintOffsetX, segFixedCoord+mazePrintOffsetY, userPathColor, MGBlockSize);
					}
					break;
					case LEFT:
					segFixedCoord = tempMazeNode->y;	// y coord doesn't change on this segment
					for (int i=tempMazeNode->x; i > tempMazeNode->prevMazeNode->x; i -= MGBlockSize) {
						tft_print_square(i+mazePrintOffsetX, segFixedCoord+mazePrintOffsetY, userPathColor, MGBlockSize);
					}
					break;
				}
				tempMazeNode = tempMazeNode->prevMazeNode;	// increment to next bend/segment
			}
			tft_print_square(MAZEPATH_BEGINX+mazePrintOffsetX, MAZEPATH_BEGINY+mazePrintOffsetY, userPathColor, MGBlockSize);	// Print tail
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			resumingProgram = 0;
			touchSenseReset();
		}
	}
	
	exitMazeGame();
}

void exitMazeGame(void)
{
	// Disable Touch Sensing
	PORTA_INT0MASK = 0x00;	// Disconnect PA4 from PORTA INT0
	PORTA_INTCTRL &= 0x00;	// Disable PORTA Interrupts
	
	if (mazeSolved) {
		if (currentMaze == 4) currentMaze = 0;	// All mazes solved, go back to first maze
		else currentMaze++;							// Send user to next maze next time they play
	
	
		// Print Win Screen //////////
		tft_print_image(MAZE_SOLVED_IMAGE_ID, userPathColor, TFT_BLACK, MAZE_SOLVED_IMAGE_STARTX, MAZE_SOLVED_IMAGE_STARTY);
		playSong(SUCCESS_SONG_ID,0,0);
		////////////////////////////////////////
		
		for (int i=0; i < 2500; i++) {	// Give user time to see the maze solved screen
			_delay_ms(1);
		}
	}
	
	// Free Node memory ////////////////////
	while (mazeHead->prevMazeNode != NULL) {
		tempMazeNode = mazeHead->prevMazeNode;
		free((mazeNode*)mazeHead);
		mazeHead = tempMazeNode;
	}
	free((mazeNode*)mazeHead);		// Free last Node
	////////////////////////////////////////
		
	currentProgram = 0;			// Go to Main Menu
	
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
			
			if (mazeHead->x == MAZEPATH_FINISHX && mazeHead->y < MAZEPATH_FINISHY) {		// Finished!
				mazeSolved = 1;
			}
		}
	}	
}



