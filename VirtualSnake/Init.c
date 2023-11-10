#include "TExaS.h" 					// znb //
#include "tm4c123gh6pm.h"   // znb //
#include "ADC.h" 						// znb // 
#include "Init.h"

//Initializing SysTick Interrupt Module
void SysTick_Init(unsigned long period){
	NVIC_ST_CTRL_R = 0;         		// disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;		// reload value
  NVIC_ST_CURRENT_R = 0;      		// any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  			// enable SysTick with core clock and interrupts
}

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
	SYSCTL_RCGC2_R;         				//    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
	SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3
	
	// Setting up Interrupts port E 
	// setting up PE0 AND PE1 to be rising edge triggered
	GPIO_PORTE_IS_R 	&= ~0x03; 		// make edge sensitive 	// 1111 1111 1111 1100 --> 0000 0000 0000 0011
	GPIO_PORTF_IBE_R 	&= ~0x03; 		// make triggered by rising edge (first click) 
	GPIO_PORTE_IEV_R 	|= 0x03; 			// 0000 0000 0000 0011
 
	GPIO_PORTE_ICR_R |= 0X03;				// clear flag
	GPIO_PORTE_IM_R |= 0X03;				// arm interrupt
	
	// Settingp Port E Interrupt Priority: source http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C12_Interrupts.htm
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFF00FFFF)|0x00A00000; // I hope this is priority 5// set up interrupt priority
	NVIC_EN0_R = 1<<4; // IRQ4 Enable PORTE interrupt
}

