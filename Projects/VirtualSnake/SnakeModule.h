#ifndef SNAKE_MODULE_H_
#define SNAKE_MODULE_H_

#include "GameEngine.h"
#include "stdbool.h"

#define TAIL_MAX_SIZE		100

typedef enum eDirection {
	STOP, LEFT, RIGHT, UP, DOWN
} Direction;

//********Snake_init*****************
// Initializes all the data found in SnakeModule.c, creates a snake
// "object" onto the project
// Inputs: Starting location of snake head X and Y, and initial tail size
// Outputs: none
// assumes: this is called in main once and will never loop again
void Snake_init(unsigned long startX, unsigned long startY, int initTailSize);

//********MoveSnake*****************
// Updates the location of the head to the tail and performs
// switch/case using eDirection to determine snake direction
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
void MoveSnake(void);

//********GrowSnake*****************
// Increments the size of the snake
// Inputs: none
// Outputs: none
// assumes: this is called once (not meant to be loooped)
void GrowSnake(void);

//********checkBodyCollision*****************
// Compares the current location of the snake with
// the location of its existing tails
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
bool checkBodyCollision(void);

//********DrawSnake*****************
// Draws the snake pixel by pixel on to the Nokia screen
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
void DrawSnake(void);

// ------- Getters and Setters -------
void setDirection(Direction curDir);
Direction getCurrentDirection(void);
unsigned long getSnakeHeadX(void);
unsigned long getSnakeHeadY(void);
unsigned int getTailLength(void);
int getCursor(void);
GTyp getTailPos(int index);

#endif


