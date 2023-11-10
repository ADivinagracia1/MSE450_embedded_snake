// TExaS Lab Graders Authors: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi

#include "TExaS.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
// Task 6 Defines
#define GPIO_PORTF_IS_R         (*((volatile unsigned long *)0x40025404))
#define GPIO_PORTF_IBE_R        (*((volatile unsigned long *)0x40025408))
#define GPIO_PORTF_IEV_R        (*((volatile unsigned long *)0x4002540C))
#define GPIO_PORTF_ICR_R        (*((volatile unsigned long *)0x4002541C))
#define GPIO_PORTF_IM_R         (*((volatile unsigned long *)0x40025410))
#define NVIC_PRI7_R             (*((volatile unsigned long *)0xE000E41C))
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))




//	Global Variables
unsigned long SW1,SW2;  

//   Function Prototypes
void PortF_Init(void);
void Control_LED(void); 			// functions you need to write
void Blinky(void);
void Delay(void); 						// functions you need to write
void EnableInterrupts(void); 	// functions you need to write
void UART_example(void); 			// An example to help you with Task 5 :)
void UART_task(void);
void Task6(void);

// 3. Subroutines Section
int main(void){    
  TExaS_Init(SW_PIN_PF40,LED_PIN_PF321); 	//	TExaS_Init initializes the real board grader for lab 4
  PortF_Init();        										// 	Call initialization of port PF   
	
	// Setting up Interrupts
	// Setting up PF4 to be negative edge activated interrupt
	GPIO_PORTF_IS_R &= ~0x10; // Make PF4 edge sensitive
	GPIO_PORTF_IBE_R |= 0x10; // Make PF4 triggered by both edges
	/*
	GPIO_PORTF_IBE_R &= ~0x10; // Make PF4 not triggered by both edges
	GPIO_PORTF_IEV_R &= ~0x10; // Make PF4 negative edge trigger
	*/
	GPIO_PORTF_ICR_R = 0x10; // Clear flag4
	GPIO_PORTF_IM_R |= 0x10; // Arm interrupt on PF4
	
	// Setting Interrupt Priority
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // Priority 5
	NVIC_EN0_R = 0x40000000; // Enable PORTF interrupt
	
	
  EnableInterrupts();
	
	while(1){
    //Task 2
		//Control_LED(); //Red LED control
		
		//Task 3
		Blinky();	//Green LED flashing
		
		//Task 4
		//UART_task();
	}
	
}

// Task 3
void Control_LED(void){
	SW1 = GPIO_PORTF_DATA_R&0x10;	//Get PF4     		
	
	if(SW1 == 0){
		GPIO_PORTF_DATA_R |= 0x02;
	}
	else{
		GPIO_PORTF_DATA_R &= 0x00;
	}
	
}

// Task 4
void Blinky(void){
	GPIO_PORTF_DATA_R ^= 0x08;
	Delay();
}
void Delay(void){
	int count = 0;
	while(count++ < 1000000);
}

// Task 5
void UART_task(void){
	char password[20];
  UART_OutChar(CR); // New line
  UART_OutChar(LF);
	
	UART_OutString("Welcome to MSE450/750!");
  UART_OutChar(CR);
  UART_OutChar(LF);
	UART_OutString("Enter password: ");
	UART_OutChar(CR);
  UART_OutChar(LF);
  UART_InString(password,19);
	
	if( strncmp(password, "password123", 11) == 0 )
		UART_OutString("Login Authenicated!");
	else
		UART_OutString("That is the wrong password");
	
	UART_OutChar(CR);
	UART_OutChar(LF);
}

// Task 6
void GPIOPortF_Handler(void){
	GPIO_PORTF_ICR_R = 0x10; // Clear flag4
	SW1 = GPIO_PORTF_DATA_R&0x10;
	
	if (!SW1){
		GPIO_PORTF_DATA_R |= 0x02;
	} 
	else if(SW1){
		GPIO_PORTF_DATA_R &= 0x00;
	}
}

// Example to help you with Task 5
void UART_example(void) {
	char string[20];
  UART_OutChar(CR); // New line
  UART_OutChar(LF);
  while(1){
    UART_OutString("Welcome to MSE450/750! Enter student ID: ");
    UART_InString(string,19);
		UART_OutChar(CR);UART_OutChar(LF);
		UART_OutString(string);
		UART_OutChar(CR);UART_OutChar(LF);
  }
}

// Initializes Port F
void PortF_Init(void){ 
  SYSCTL_RCGC2_R 		|= 0x00000020;     	// 1) F clock
  GPIO_PORTF_LOCK_R  = 0x4C4F434B;   		// 2) unlock PortF PF0  
  GPIO_PORTF_CR_R 	 = 0x1F;           	// allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        		// 3) disable analog function
  GPIO_PORTF_PCTL_R  = 0x00000000;   		// 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R 	 = 0x0E;          	// 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;       	 		// 6) no alternate function
  GPIO_PORTF_PUR_R 	 = 0x11;          	// enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R 	 = 0x1F;          	// 7) enable digital pins PF4-PF0    	
}


