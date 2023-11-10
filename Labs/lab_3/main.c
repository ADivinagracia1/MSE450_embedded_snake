// MSE 450 - Lab 3
// Group 18

#include "TExaS.h"
#include "UART.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//Prototypes
//Task 1
void ADC0_Init(void);
unsigned long ADC0_In(void);
unsigned long ADCValue;

//Task 2
void PortF_Init(void);

//Task 4
void DAC_Init(void);
void SysTickInt_Init(unsigned long period);
void DAC_Out(unsigned long data);
unsigned char Index;
const unsigned char Wave[8] = {0, 1, 2, 3, 4, 5, 6, 7};

int main(void){
	
	//Task 1
 	ADC0_Init();
	
	//Task 2
	PortF_Init();
	UART_Init();

	//Task 4
	SysTickInt_Init(50000); //100Hz
	
	while(1){
		ADCValue = ADC0_In();
	}
}

// SAMPLE CODE 
// This initialization function sets up the ADC 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source:  channel 1
// SS3 interrupts: enabled but not promoted to controller
void ADC0_Init(void){ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E - Pg. 431
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0  - Pg.425
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3 - Pg. 803
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger - Pg. 797
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2) - Pg. 837
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0 - Pg. 838
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3 - Pg. 785
}


//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_In(void){  
  unsigned long result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3 - Pg. 807
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done - Pg. 787
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result - Pg. 822
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion - Pg. 792
  return result;
}


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

void GPIOPortF_Handler(void){
	unsigned long SW2 = GPIO_PORTF_DATA_R&0x01; 
	
	if (!SW2){
  UART_OutChar(CR); // New line
  UART_OutChar(LF);
	UART_OutString("ADC Value in Hex: 0x");
	UART_OutUHex(ADC0_In());

	} 
	GPIO_PORTF_ICR_R = 0x01; // Clear flag4
}

//Task 4 Functions
//Worked from Example code in C13_R2R_3bit in TExaSware Folder

// **************DAC_Init*********************
// Initialize 3-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0x07;      // no analog 
  GPIO_PORTB_PCTL_R &= ~0x00000FFF; // regular function
  GPIO_PORTB_DIR_R |= 0x07;      // make PB2-0 out
  GPIO_PORTB_AFSEL_R &= ~0x07;   // disable alt funct on PB2-0
  GPIO_PORTB_DEN_R |= 0x07;      // enable digital I/O on PB2-0
}

//SysTick Interrupt Init
void SysTickInt_Init(unsigned long period){
  DAC_Init();          // Port B is DAC
  Index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}

// **************DAC_Out*********************
// output to DAC
// Input: 3-bit data, 0 to 7 
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data;
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
  GPIO_PORTF_DATA_R ^= 0x08;     // toggle PF3, debugging
  Index = (Index+1)%0x07; //Changed this to work with circular array like the one from project 
  DAC_Out(Wave[Index]); 
}




/*

// write your prototypes here
void PortF_Init(void); //Initializing and Configuring PortF
// For Task 1
void Task1_Blink(void); 
void SysTick_Init(void);
void SysTick_Wait(uint32_t delay);
void SysTick_Delay(void);
// For Task 2
void Task0_Blink(void);
void Timer0_Init(uint32_t period);
void EnableInterrupts(void);
// For Bonus
void PWM0A_Init(uint16_t period_PWM, uint16_t duty_PWM);

uint32_t period = 8000000; // period*12.5ns = delay time
uint16_t period_PWM = 40000; //1000kHz 
uint16_t duty_PWM = 30000; // 75% Duty Cycle

int main (void){
	//write your code here
	TExaS_Init(SW_PIN_PF40,LED_PIN_PF321); 	//	TExaS_Init initializes the real board grader for lab 4
  PortF_Init();        										// Call initialization of port PF   
	
	//Initialization
	//Task 1
	SysTick_Init(); // Setting up SysTick
	
	//Task 2
	Timer0_Init(period);
	
	//Bonus
	//PWM0A_Init(period_PWM, duty_PWM);
	
	while(1){
	// Task 1
	Task1_Blink();
	
	}
}

// Functions for Task 1
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0; //Disable SysTick during setup
	NVIC_ST_CTRL_R = 0x00000005; // Picking the System Clock and Enabling SysTick
}
void SysTick_Wait(uint32_t delay){
	NVIC_ST_RELOAD_R = delay - 1; //Number of Counts to Wait
	NVIC_ST_CURRENT_R = 0; //Clear any value written to CURRENT
	while((NVIC_ST_CTRL_R&0x00010000) == 0){} //Wait for Count Flag in Bit 16 of CTRL register
}

void SysTick_Delay(void){
	uint32_t delay = 10; //10*100ms = 1000ms
	uint32_t count;
	for(count = 0; count<delay; count++){
		SysTick_Wait(8000000); //Wait 100ms, 800000/80MHz
	}
}

void Task1_Blink(void){
	GPIO_PORTF_DATA_R ^= 0x04;
	SysTick_Delay();
}

// Functions for Task 2
void Timer0_Init(uint32_t period){
	//period*12.5ns
	SYSCTL_RCGCTIMER_R |= 0x01; //Activate Timer2
	TIMER0_CTL_R = 0x00000000; //Disable Timer0A while setting up
	TIMER0_CFG_R = 0x00000000; //Use 32-bit mode
	TIMER0_TAMR_R = 0x00000002; //Use Periodic Mode
	TIMER0_TAILR_R = period-1; //Reload Starting Value
	TIMER0_TAPR_R = 0; //Clock Resolution at 12.5 ns
	TIMER0_ICR_R = 0x00000001; //Clear Timer0A Timeout Flag
	TIMER0_IMR_R = 0x00000001; //Arm Timeout Interrupt
	NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; //Set Priority
	NVIC_EN0_R = 1<<19; //Enable IRQ19 in NVIC
	TIMER0_CTL_R = 0x00000001; //Re-enable Timer0A
	//EnableInterrupts();
}

void Timer0A_Handler(void){
	if(TIMER0_MIS_R & 0x1){
		//Function To Carry Out
		GPIO_PORTF_DATA_R ^= 0x02; //Blink LED
		TIMER0_ICR_R = 0x00000001; //Acknowledge Timer0A Timeout
	}
}


// Initializes Port F
void PortF_Init(void){ 
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0
}

*/
