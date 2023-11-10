#include "SnakeModule.h"
#include "config.h"
#include "stdbool.h"
#include "Nokia5110.h"
#include <string.h>
#include <stdio.h> 				// znb //
#include "tm4c123gh6pm.h" // znb //

static Direction currentDirection;
static GTyp snake_head;
static GTyp snake_tail[TAIL_MAX_SIZE];
static unsigned int currentTailSize;		 	// Tracks length of tail
static int tail_cusor;										// Points to the index of array

//Connect the snake to a sprite 
/*
struct SnakeSprite {
	GTyp GObj;
	const unsigned char *img;
};
typedef struct SnakeSprite STyp;

*/

//********Snake_init*****************
// Initializes all the data found in SnakeModule.c, creates a snake
// "object" onto the project
// Inputs: Starting location of snake head X and Y, and initial tail size
// Outputs: none
// assumes: this is called in main once and will never loop again
void Snake_init(unsigned long startX, unsigned long startY, int initTailSize)
{
	unsigned int i;
	snake_head.x = startX;																// initialize head location X
	snake_head.y = startY;																// initialize head location Y
	memset(snake_tail, 0, TAIL_MAX_SIZE * sizeof(GTyp));	// Set all tail array values to 0 (Note: tracks the history of the head)
	currentTailSize = initTailSize;												// Track initial tail size 
	tail_cusor = currentTailSize;													// Tracks to the index of array [cursor]
	currentDirection = LEFT;															// Initial movement direction	
	
	//Create starting tail 
	for (i = 0; i < initTailSize; i++){
		snake_tail[i].x = startX + ( initTailSize - i );		// Tails appear to the right of the snake
		snake_tail[i].y = startY;
	}
}

//********MoveSnake*****************
// Updates the location of the head to the tail and performs
// switch/case using eDirection to determine snake direction
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
void MoveSnake(void){
	
	//Updates Tail - puts old head into the tail
	snake_tail[tail_cusor].x = snake_head.x;
	snake_tail[tail_cusor].y = snake_head.y;
	tail_cusor = (tail_cusor + 1) % TAIL_MAX_SIZE;	// Initializes circular array and increments the tail cursor location
																									// Note: 3%5 = 3, 5%5 = 0
	
	//Apply direction 
	switch(currentDirection){
		case LEFT: 			//Left Direction
			snake_head.x--;
			break;
		case RIGHT: 		//Right Direction
			snake_head.x++;
			break;
		case UP: 				//Up direction
			snake_head.y--;
			break;
		case DOWN:			//Down Direction
			snake_head.y++;
			break;
		case STOP:
			break;
	}
}

//********GrowSnake*****************
// Increments the size of the snake
// Inputs: none
// Outputs: none
// assumes: this is called once (not meant to be loooped)
void GrowSnake(void){
	currentTailSize++;
}

//********checkBodyCollision*****************
// Compares the current location of the snake with
// the location of its existing tails
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
bool checkBodyCollision(void){
	unsigned char i;
	unsigned long x_check = snake_head.x;
	unsigned long y_check = snake_head.y;

	for(i = 0; i < currentTailSize; i++){
		
		if((x_check == snake_tail[tail_cusor-i].x)&&(y_check == snake_tail[tail_cusor-i].y)){
			return true; //Return true if snake crosses itself
		}
	}
	return false; 
}

//********DrawSnake*****************
// Draws the snake pixel by pixel on to the Nokia screen
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
void DrawSnake(void){
	int i;
	int draw_i = tail_cusor - 1;
	
	// Draw Head
	Nokia5110_SetPixel(snake_head.x, snake_head.y);

	// Draw Tail <DO NOT TOUCH>
	for(i = 0; i < currentTailSize; i++){
	// The tail array is a CIRCULAR array (array that goes to 0 once it hits the end)
	// Counting from cursor of the tail array and looping over once it goes out of bounds (aka decrements to 0)
	// Counter returns to top of array (not maximum value)
		
		if ( draw_i < 0 )
			draw_i = TAIL_MAX_SIZE - 1;
		
		Nokia5110_SetPixel(snake_tail[draw_i].x, snake_tail[draw_i].y);
		draw_i--;
		
	} // for
} // DrawSnake

// Getters and Setters ===========================================

void setDirection(Direction curDir){
	currentDirection = curDir;
}

Direction getCurrentDirection(void){
	return currentDirection;
}

unsigned long getSnakeHeadX(void){
	return snake_head.x;
}

unsigned long getSnakeHeadY(void){
	return snake_head.y;
}

unsigned int getTailLength(void){
	return currentTailSize;
}

int getCursor(void){
	return tail_cusor;
}

GTyp getTailPos(int index){
	return snake_tail[index];
}
