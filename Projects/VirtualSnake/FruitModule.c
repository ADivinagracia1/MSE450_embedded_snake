#include "FruitModule.h"
#include "GameEngine.h"
#include "random.h"
#include "Nokia5110.h"
#include "config.h"
#include "SnakeModule.h"
#include "stdbool.h"

#define SRAND_SEED	420

static GTyp Fruit;				// Fruit "object"
unsigned char fruitFlag;	// Flag that checks if a fruit is on the screen

// Private FruitModule functions
// used in checkFruitCollission()
void checkFruitX(void);
bool checkFruitY(unsigned char i);

//BMP of Fruit
const unsigned char Fruit_BMP[] = {

0x42,0x4D,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x00,0x00,0x00,0x28,0x00,

0x00,0x00,

0x08,0x00,0x00,0x00,  // width is 8 pixels

0x08,0x00,0x00,0x00,  // height is 8 pixels

0x01,0x00,0x04,0x00,0x00,0x00,

0x00,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x80,

0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x80,

0x00,0x00,0x80,0x80,0x80,0x00,0xC0,0xC0,0xC0,0x00,0x00,0x00,0xFF,0x00,0x00,0xFF,

0x00,0x00,0x00,0xFF,0xFF,0x00,0xFF,0x00,0x00,0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFF,

0x00,0x00,0xFF,0xFF,0xFF,0x00,

0x00,0x00,0x00, 0x00,	//row7 - Bottom

0x00,0x0F,0xF0, 0x00,	//row6

0x00,0xF0,0x0F, 0xF0,	//row5

0x00,0xF0,0x00, 0xF0,	//row4

0x00,0x0F,0xFF, 0x00,	//row3

0x00,0x0F,0x00, 0x00,	//row2

0x0F,0xF0,0xFF, 0xF0,	//row 1

0x00,0x00,0x00, 0x00,  // top row - row 0

0xFF};


//********Fruit_init*****************
// Assigns values to all the data in FruitModule.c
// Initializes a fruit "object" into the project
// Inputs: Screen size X and Y
// Outputs: none
// assumes: this is meant to be called once in initialzation
void Fruit_init(unsigned int mapMaxX, unsigned int mapMaxY){
	
	Random_Init(SRAND_SEED);		// Initialize random variable making 
	Fruit.x = mapMaxX/2;				// Set initial location of fruit X
	Fruit.y = mapMaxY/2-10;			// Set initial location of fruit Y
	fruitFlag = 0;							// Spawn a new fruit from beginning
}

//********fruitPlace*****************
// Places fruit in a random location in the screen
// Initializes a fruit "object" into the project
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
void fruitPlace(unsigned int mapMaxX, unsigned int mapMaxY){
	
	if (fruitFlag == 0){
		//Fruit.x = Random()% (mapMaxX-9) + 1;			// Spawn fruit in X = 1 to 75
		//Fruit.y = Random()% (mapMaxY-9) + 1;			// Spawn fruit in Y = 1 to 29
		
		fruitFlag = 1; //Can probably put this into struct later on
	}
	
	Nokia5110_PrintBMP(Fruit.x,Fruit.y,Fruit_BMP,0);
}

//********checkFruitCollission*****************
// Takes information about the snake head compares with the 
// location X and Y area of the fruit 
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
// NOTE: Could be done with interrupts
void checkFruitCollission(void){
	checkFruitX();
}

// Private function used in checkFruitCollission
void checkFruitX(void){
	unsigned char i;
	
	for(i = getFruitX(); i < (getFruitX()+FRUIT_WIDTH); i++){
		if(checkFruitY(i)){
			fruitFlag = 0;
			Fruit.x = Random()% (BORDER_X-9) + 1;			// Spawn fruit in X = 1 to 75
			Fruit.y = Random()% (BORDER_Y-9) + 1;			// Spawn fruit in Y = 1 to 29
			GrowSnake();
			break;
		}
	}
}

// Private function used in checkFruitX
bool checkFruitY(unsigned char i){
	unsigned char j;
	
	for(j = getFruitY(); j > (getFruitY()-FRUIT_HEIGHT); j--){
		if((getSnakeHeadX()==i)&&(getSnakeHeadY()==j)){
			return true;
		}
	}
	return false;
}

// Getters =========================================
unsigned long getFruitX(void){
	return Fruit.x;
}

unsigned long getFruitY(void){
	return Fruit.y;
}
