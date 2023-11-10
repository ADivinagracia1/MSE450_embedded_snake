// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is oritinally a starter project for the edX Lab 15, 
// now used for MSE450 - Real Time Embedded Systems with Amr Marzouk 
/*
 * NOKIA SNAKE GAME by Group 18
 *	- Andrei Divinagracia		301360608
 *	- Dexter Zamora					301344653
 *	- Zainab Vajahath 			301238390
 */

#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"

#include "SnakeModule.h"
#include "FruitModule.h"
#include "config.h"
#include <stdio.h>
#include "Init.h"
#include "ADC.h"

//Body and Border Collission Test 
void checkGameOver(void);
void DrawBorder(void);

unsigned long period = 8000000; 	// Interrupts at 100ms, period/80MHz 
unsigned char gameOverState = 0;	// Game over flag

unsigned long SW1,SW2, LEFT_ADC, RIGHT_ADC; // Hardware button names


int main(void){ TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
  // you cannot use both the Scope and the virtual Nokia (both need UART0)
  Random_Init(1);
  Nokia5110_Init();
	PortF_Init(); 					// znb //
	PortE_Init(); 					// znb //
	SysTick_Init(period); 	// Initialize SysTick with periodic interrupts
	
	// Initialize game "objects"
	Snake_init(BORDER_X/2, BORDER_Y/2, TAIL_SIZE_INIT);
	Fruit_init(BORDER_X, BORDER_Y);
	
	while(1){
		//Game runs according to SysTick timer
  }

} //main

// Primary Interrupt Handler ===============================================
void SysTick_Handler(void){

	//Draw game frames
	Nokia5110_ClearBuffer(); // ----- Clear buffer ------
	
	//Check game over
	checkGameOver();
	
	//Keep normal gameplay if game is not over
	if(gameOverState == 0){
		MoveSnake();
		DrawBorder();
		DrawSnake();				
		fruitPlace(BORDER_X,BORDER_Y);
		checkFruitCollission();
	}
	else {
		DrawBorder();
		Nokia5110_SetCursor(1, 1);
		Nokia5110_OutString("GAME OVER");
		Nokia5110_SetCursor(1, 2);
		Nokia5110_OutString("SNEK DIED!");
	}
	
	Nokia5110_DisplayBuffer(); // ----- Draw Buffer -----
	
} // SysTick_Handler

// PORT E ISR =============================================================
void GPIOPortE_Handler(void) {
	
	GPIO_PORTE_ICR_R |= 0X03;						// clear flag
	GPIO_PORTE_DIR_R &= ~0x07;        	// PE2,1,0 input 
  GPIO_PORTE_PCTL_R &= ~0x000000F0; 	// bits for PE1,PE0 are being set to zero
	LEFT_ADC = GPIO_PORTE_DATA_R&0x02;
	RIGHT_ADC = GPIO_PORTE_DATA_R&0x01;

	if(LEFT_ADC)
		setDirection(LEFT);
	
	if(RIGHT_ADC)
		setDirection(RIGHT);

}	// GPIOPortE_Handler

// PORT F ISR =============================================================
void GPIOPortF_Handler(void){
	
	GPIO_PORTF_ICR_R = 0x10; // Clear flag4
	SW1 = GPIO_PORTF_DATA_R&0x10;
	SW2 = GPIO_PORTF_DATA_R&0x01;

	//if (!SW2 && getCurrentDirection() != UP)
	if (!SW2)	
		setDirection(DOWN);
	
	//if(!SW1 && getCurrentDirection() != DOWN) // Does not work as intended
	if(!SW1)
		setDirection(UP);
	
} // GPIOPortF_Handler

// Draw outline of Nokia screen
void DrawBorder(void){
	int i, j;
	for(i = 0; i < BORDER_Y; i++){
		for(j = 0; j < BORDER_X; j++){
			if((i == 0) || (i == BORDER_Y - 1) || (j == 0) || (j == BORDER_X - 1))
				Nokia5110_SetPixel(j,i);
			else 
				continue;
		} // for
	} // for
} // Draw border


//Checking if Game Over Conditions are met
void checkGameOver(void){
	unsigned long x = getSnakeHeadX();
	unsigned long y = getSnakeHeadY();
	
	//Checking if head position is in border
	if((y == 0) || (y == BORDER_Y - 1) || (x == 0) || (x == BORDER_X - 1))
		gameOverState = 1;
	
	//Checking if head crosses its own body
	else if(checkBodyCollision())
		gameOverState = 1;
	
} // checkGameOver

