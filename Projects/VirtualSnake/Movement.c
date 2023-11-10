//MSE 450 - Snake Project

#include "Movement.h"
#include "Nokia5110.h"

//Remove this after snake object is made
//Variables to test SnakeMove in Movement.C
// Maximum dimensions of the LCD
#define BORDER_X				84
#define BORDER_Y				48
#define START_X					BORDER_X/2
#define START_Y					BORDER_Y/2
int pos_x = START_X;
int pos_y = START_Y;

//Snake Movement Mechanism
//Input is the current direction to travel
//Output is the movement of the snake on Nokia5510 screen
void SnakeMove(char direction){ //DELETE THIS AFTER SNAKE OBJECT IS MADE
//Comment in below for when snake object is made
//void SnakeMove(char direction, int pos_x, int pos_y){
	
	//REMOVE THIS LATER, just want to see the pixel move
	//Clear pixel at current location
	Nokia5110_ClearPixel(pos_x, pos_y); 

	//Get direction 
	switch(direction){
	
		case '0': //Left Direction
			pos_x--;
			break;
		case '1': //Right Direction
			pos_x++;
			break;
		case '2': //Up direction
			pos_y--;
			break;
		case '3':	//Down Direction
			pos_y++;
			break;
	}
	
	//Set pixel at new location of snake head
	Nokia5110_SetPixel(pos_x,pos_y);
}

