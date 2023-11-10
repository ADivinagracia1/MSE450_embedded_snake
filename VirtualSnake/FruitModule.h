#ifndef FRUIT_MODULE_H_
#define FRUIT_MODULE_H_
#include "stdbool.h"

//********Fruit_init*****************
// Assigns values to all the data in FruitModule.c
// Initializes a fruit "object" into the project
// Inputs: Screen size X and Y
// Outputs: none
// assumes: this is meant to be called once in initialzation
void Fruit_init(unsigned int mapMaxX, unsigned int mapMaxY);

//********fruitPlace*****************
// Places fruit in a random location in the screen
// Initializes a fruit "object" into the project
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
void fruitPlace(unsigned int mapMaxX, unsigned int mapMaxY);

//********checkFruitCollission*****************
// Takes information about the snake head compares with the 
// location X and Y area of the fruit 
// Inputs: none
// Outputs: none
// assumes: this is meant to be called once per clock cycle
void checkFruitCollission(void);

// ---------- Getters ----------
unsigned long getFruitX(void);
unsigned long getFruitY(void);

#endif
