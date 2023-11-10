// TExaS Lab Graders Authors: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi

#include "TExaS.h"
#include "UART.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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

// Bonus Task
// Sample Code
void PWM0A_Init(uint16_t period_PWM, uint16_t duty_PWM){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
/*  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){};
  GPIO_PORTB_AFSEL_R |= 0x40;           // enable alt funct on PB6
  GPIO_PORTB_PCTL_R &= ~0x0F000000;     // configure PB6 as PWM0
  GPIO_PORTB_PCTL_R |= 0x04000000;
  GPIO_PORTB_AMSEL_R &= ~0x40;          // disable analog functionality on PB6
  GPIO_PORTB_DEN_R |= 0x40;             // enable digital I/O on PB6
*/ 
	//Have to pick a pin and configure it out as PWM output
	SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
      (SYSCTL_RCC_R & (~0x000E0000));   //    configure for /2 divider
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PB6 goes low on LOAD
  // PB6 goes high on CMPA down
  PWM0_0_LOAD_R = period_PWM - 1;           // 5) cycles needed to count down to 0
  PWM0_0_CMPA_R = duty_PWM - 1;             // 6) count value when output rises
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000001;          // enable PB6/M0PWM0
}

void PWM0A_Duty(uint16_t duty){
	PWM0_0_CMPA_R = duty - 1; // 6) count value when output rises
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


