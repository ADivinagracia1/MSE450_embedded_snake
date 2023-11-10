// TExaS Lab Graders Authors: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi

/*
GOAL: Add Port F and Port E

notes: looks like LEFT and RIGHT on ADC are not latching 

*/
#include "TExaS.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

//includes for ADC game function
//#include "ADCSWTrigger.h"
//#include "GameEngine.h"
#include "tm4c123gh6pm.h"
//#include "PLL.h"
#include "ADC.h"

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
unsigned long SW1,SW2, LEFT, RIGHT;
unsigned long test;
unsigned long delay;

//   Function Prototypes
void PortF_Init(void);
void PortE_Init(void);
void Control_LED(void); 			// functions you need to write
void Blinky(void);
void Delay(void); 						// functions you need to write
void EnableInterrupts(void); 	// functions you need to write
void UART_example(void); 			// An example to help you with Task 5 :)
void UART_task(void);
void Task6(void);
void TestADC(void);

//Function Prototypes to Incorporate both buttons 
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void SwitchInit(void);

// 3. Subroutines Section
int main(void){    
	DisableInterrupts(); // just like completed space invaders
  TExaS_Init(SW_PIN_PF40,LED_PIN_PF321); 	//	TExaS_Init initializes the real board grader for lab 4
  PortF_Init();        										// 	Call initialization of port PF   
	PortE_Init();
	ADC0_Init(); //not sure what this function does actually
	//ADC_Input();

	// Setting up Interrupts
	// Setting up PF4 to be negative edge activated interrupt
	GPIO_PORTF_IS_R &= ~0x11; // Make PF4,PF0 edge sensitive
	GPIO_PORTF_IBE_R |= 0x11; // Make PF4, PF0 triggered by both edges

	GPIO_PORTF_ICR_R = 0x10; // Clear flag4 // not sure if this should be 0x10 or 0x00
	GPIO_PORTF_IM_R |= 0x11; // Arm interrupt on PF4, PF0
	
	// Settingp Port F Interrupt Priority
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // Priority 5
	//NVIC_EN0_R = 0x40000000; // Enable PORTF interrupt
	
	//hehe enabling all interrupts bc I have no chill, which one is port R interrupt ??
		NVIC_EN0_R = 0xFFFFFFFF; // Enable interrupt
	
	// Settingp Port E Interrupt Priority: source http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C12_Interrupts.htm
	NVIC_PRI1_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // I hope this is priority 5
		
  EnableInterrupts();
	
	while(1){
		
		
//	test = ADC0_InitSWTriggerSeq3_Ch1();
    //Task 2
		//Control_LED(); //Red LED control
		
		//Task 3
		//Blinky();	//Green LED flashing
		
		//Task 4
		//UART_task();
		
	}
	
}

///////////////////////////////////////////////////////////

// Task 3
void Control_LED(void){
	
	SW1 = GPIO_PORTF_DATA_R&0x10;	//Get PF4
	SW2 = GPIO_PORTF_DATA_R&0x01;	// Get PF0
	
	if(SW2 == 0){
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
//	UART_OutString("Interrupt for SW1 @ PF4");
//	UART_OutChar(CR);
//  UART_OutChar(LF);
	
	GPIO_PORTF_ICR_R = 0x10; // Clear flag4
	SW1 = GPIO_PORTF_DATA_R&0x10;
	SW2 = GPIO_PORTF_DATA_R&0x01;
	
	if (!SW2){
		GPIO_PORTF_DATA_R |= 0x02;
		UART_OutString("Interrupt for SW2 at PF0");
		UART_OutChar(CR);
		UART_OutChar(LF);
	} 
	else if(SW2){
		GPIO_PORTF_DATA_R &= 0x00;
	}
}

void GPIOPortE_Handler(void) {
	// how do I clear the flag for this?
	GPIO_PORTE_DIR_R &= ~0x07;        // PE2,1,0 input
	LEFT = GPIO_PORTE_PCTL_R &= ~0x000000F0; // bits for PE1,PE0
	RIGHT = GPIO_PORTE_PCTL_R &= ~0x0000000F; // bits for PE1,PE0
	
	if(LEFT) {
		UART_OutChar(CR);
  UART_OutChar(LF);
	UART_OutString("Interrupt for ADC");
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

void Switch_Init(void){  
	//uint32_t volatile delay;

  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F

  //delay = SYSCTL_RCGCGPIO_R;
	Delay();

  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F

  GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0

  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)

  GPIO_PORTF_DEN_R |= 0x11;     //     enable digital I/O on PF4,0

  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0

  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive

  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges

  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event

  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0

  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0

  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2 ///////////// we should use this

  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC

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



// Initalize Port E // 
void PortE_Init(void) {
  SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3
}


/* alternatively could try this
void ADC0_InitSWTriggerSeq3_Ch9(void){
  SYSCTL_RCGCADC_R |= 0x0001;   // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x10;    // 2) activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x10) != 0x10){};  // 3 for stabilization
  GPIO_PORTE_DIR_R &= ~0x10;    // 4) make PE4 input
  GPIO_PORTE_AFSEL_R |= 0x10;   // 5) enable alternate function on PE4
  GPIO_PORTE_DEN_R &= ~0x10;    // 6) disable digital I/O on PE4
  GPIO_PORTE_AMSEL_R |= 0x10;   // 7) enable analog functionality on PE4
// while((SYSCTL_PRADC_R&0x0001) != 0x0001){}; // good code, but not implemented in simulator
  ADC0_PC_R &= ~0xF;
  ADC0_PC_R |= 0x1;             // 8) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;        // 9) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;      // 10) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;       // 11) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;
  ADC0_SSMUX3_R += 9;           // 12) set channel
  ADC0_SSCTL3_R = 0x0006;       // 13) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;         // 14) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;       // 15) enable sample sequencer 3
}
*/
