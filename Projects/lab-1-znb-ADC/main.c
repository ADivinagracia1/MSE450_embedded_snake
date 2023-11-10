// TExaS Lab Graders Authors: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi

/*
GOAL: Add Port F and Port E
notes: looks like LEFT and RIGHT on ADC are not latching BUT SW1 and SW2 are latching
				
*/
#include "TExaS.h" // znb //
#include "UART.h"  // znb //
#include <stdio.h> // znb //
#include <string.h> // znb //

#include "tm4c123gh6pm.h"  //znb //
#include "ADC.h" //znb // 


//	Global Variables
unsigned long SW1,SW2, LEFT, RIGHT; // znb //
//unsigned long test;
unsigned long delay; // znb//

//Function Prototypes to Incorporate both buttons 
void DisableInterrupts(void); // znb //  Disable interrupts
void EnableInterrupts(void);  // znb //  Enable interrupts
void SwitchInit(void); // znb //
void PortF_Init(void); // znb //
void PortE_Init(void); // znb //


// 3. Subroutines Section
int main(void){    
	
	
	
//  TExaS_Init(SW_PIN_PF40,LED_PIN_PF321); 	//	TExaS_Init initializes the real board grader for lab 4
	 // znb all code below // 
	DisableInterrupts(); // just like completed space invaders
  PortF_Init();  // 	Call initialization of port PF   
	PortE_Init();	// 	Call initialization of port PE
	ADC0_Init();
  EnableInterrupts();
	
	while(1){
		
	
	}
	
}

///////////////////////////////Functions

// Handles when Port F button SW1 or SW2 is pressed, and will out put a string to UART
// Button is LATCHING and needs to be rising and falling edge triggered only
void GPIOPortF_Handler(void){ // znb // 
//	UART_OutString("Interrupt for SW1 @ PF4");
//	UART_OutChar(CR);
//  UART_OutChar(LF);
	
	GPIO_PORTF_ICR_R = 0x10; // Clear flag4
	SW1 = GPIO_PORTF_DATA_R&0x10;
	SW2 = GPIO_PORTF_DATA_R&0x01;
	
	// setup the interupts in a way that will only trigger by edge instead of enabling
	if (!SW2){
		UART_OutString("Interrupt for SW2 at PF0");
		UART_OutChar(CR);
		UART_OutChar(LF);
	} else if(SW2){
		GPIO_PORTF_DATA_R &= 0x00;
	}
	
	if(!SW1) {
				UART_OutString("Interrupt for SW1 at PF1 !!!");
			UART_OutChar(CR);
			UART_OutChar(LF);
	} else if (SW1) {
		// do nothing
	}
	
} // end function

// Handles when ADC button LEFT or RIGHT is pressed and will output string to UART
// Buttons are not latching
void GPIOPortE_Handler(void) { //  znb // 
// switches are read ports, LEDs are write ports
	GPIO_PORTE_ICR_R |= 0X03;	// clear flag
	GPIO_PORTE_DIR_R &= ~0x07;        // PE2,1,0 input //setting bits to 0 is input // 7 in binary is 0000 0000 0000 0000 0000 0000 0000 0111
  GPIO_PORTE_PCTL_R &= ~0x000000F0; // bits for PE1,PE0 are being set to zero
	LEFT = GPIO_PORTE_DATA_R&0x02;
	RIGHT = GPIO_PORTE_DATA_R&0x01;

	// LEFT is pressed, output string to UART
	if(LEFT) {
		UART_OutString("Interrupt for LEFT == 0");
		UART_OutChar(CR);
		UART_OutChar(LF);
		GPIO_PORTF_DATA_R ^= 0x08;
	} 
	
	// RIGHT is pressed, output string to UART
	if(RIGHT) {
		UART_OutString("Interrupt for RIGHT == 0");
		UART_OutChar(CR);
		UART_OutChar(LF);
		GPIO_PORTF_DATA_R ^= 0x08;
	} 

}

//////////////////// initalization code below

// Initializes Port F 
void PortF_Init(void){  // znb // 
  SYSCTL_RCGC2_R 		|= 0x00000020;     	// 1) F clock
  GPIO_PORTF_LOCK_R  = 0x4C4F434B;   		// 2) unlock PortF PF0  
  GPIO_PORTF_CR_R 	 = 0x1F;           	// allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        		// 3) disable analog function
  GPIO_PORTF_PCTL_R  = 0x00000000;   		// 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R 	 = 0x0E;          	// 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;       	 		// 6) no alternate function
  GPIO_PORTF_PUR_R 	 = 0x11;          	// enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R 	 = 0x1F;          	// 7) enable digital pins PF4-PF0    	
	
	// Setting up Interrupts port F
	// Setting up PF4 AND PF0 to be positive edge activated interrupt
	GPIO_PORTF_IS_R &= ~0x11; // Make PF4,PF0 edge sensitive
	GPIO_PORTF_IBE_R |= 0x11; // Make PF4, PF0 triggered by both edges

	GPIO_PORTF_ICR_R = 0x11; // Clear flag4 for both switches
	GPIO_PORTF_IM_R |= 0x11; // Arm interrupt on PF4, PF0
	
	// Settingp Port F Interrupt Priority
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // Priority 5
	NVIC_EN0_R = 0x40000000; // Enable PORTF interrupt
}


// Initalize Port E // 
void PortE_Init(void) {	// znb // 
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
	
	// Setting up Interrupts port E 
	// setting up PE0 AND PE1 to be rising edge triggered
	GPIO_PORTE_IS_R &= ~0x03; // make edge sensitive 	// 1111 1111 1111 1100 --> 0000 0000 0000 0011
	GPIO_PORTF_IBE_R &= ~0x03; // make triggered by rising edge (first click) 
	GPIO_PORTE_IEV_R |= 0x03; // 0000 0000 0000 0011
 
	
	GPIO_PORTE_ICR_R |= 0X03;	// clear flag
	GPIO_PORTE_IM_R |= 0X03;// arm interrupt
	
	// Settingp Port E Interrupt Priority: source http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C12_Interrupts.htm
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFF00FFFF)|0x00A00000; // I hope this is priority 5// set up interrupt priority
	NVIC_EN0_R = 1<<4; // IRQ4 Enable PORTE interrupt
}


//// Task 3
//void Control_LED(void){
//	
//	SW1 = GPIO_PORTF_DATA_R&0x10;	//Get PF4
//	SW2 = GPIO_PORTF_DATA_R&0x01;	// Get PF0
//	
//	if(SW2 == 0){
//		GPIO_PORTF_DATA_R |= 0x02;

//	}
//	else{
//		GPIO_PORTF_DATA_R &= 0x00;
//	}
//	
//}


// code I found from textbook
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

// code I found from textbook
//void Switch_Init(void){  
//	//uint32_t volatile delay;

//  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F

//  //delay = SYSCTL_RCGCGPIO_R;
//	Delay();

//  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F

//  GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0

//  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)

//  GPIO_PORTF_DEN_R |= 0x11;     //     enable digital I/O on PF4,0

//  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0

//  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive

// // GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges

////  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event

//  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0

//  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0

//  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2 ///////////// we should use this

//  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC

//}

