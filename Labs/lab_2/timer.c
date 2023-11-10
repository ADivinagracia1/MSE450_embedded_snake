// TExaS Lab Graders Authors: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi

#include "TExaS.h"
#include "UART.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <string.h>

void (*PeriodicTask)(void);   // user function

// ***************** Timer0_Init ****************
// Activate TIMER0 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none


void Timer0_Init(void);
void timer0A_delayMs(int);

int main (void)
{
	TExaS_Init(SW_PIN_PF40,LED_PIN_PF321);
	Timer0_Init();


	while(1)
	{
			GPIO_PORTF_DATA_R = 2;        /* turn on red LED */
			timer0A_delayMs(500);   /* TimerA 500 msec delay */
			GPIO_PORTF_DATA_R = 0;        /* turn off red LED */
			timer0A_delayMs(500);   /* TimerA 500 msec delay */
		
	}
 
}

//void timer0_init(){
//	/* enable clock to GPIOF at clock gating control register */
//	SYSCTL_RCGC2_R |= 0x00000020;
//	SYSCTL_RCGCGPIO_R |= 0x20;
//	/* enable the GPIO pins for the LED (PF3, 2 1) as output */
//	GPIO_PORTF_DIR_R = 0x0E;
//	/* enable the GPIO pins for digital function */
//	GPIO_PORTF_DEN_R = 0x0E;
//	SYSCTL_RCGCTIMER_R |= 1;     /* enable clock to Timer Block 0 */
//	TIMER0_CTL_R = 0x00000000;            /* disable Timer before initialization */
//	TIMER0_CFG_R = 0x04;         /* 16-bit option */
//	TIMER0_TAMR_R = 0x02;        /* periodic mode and down-counter */
//	TIMER0_TAILR_R = 16000 - 1;  /* Timer A interval load value register */
//	TIMER0_ICR_R = 0x1;          /* clear the TimerA timeout flag*/
//	TIMER0_CTL_R |= 0x01;        /* enable Timer A after initialization */
//}

void Timer0_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
//  PeriodicTask = task;          // user function
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = 16000-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
}

//void Timer0A_Handler(void){
//  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER0A timeout
//  (*PeriodicTask)();                // execute user task
//}
 
/* multiple of millisecond delay using periodic mode */
void timer0A_delayMs(int ttime)
{
    int i;
    for(i = 0; i < ttime; i++) { while ((TIMER0_RIS_R & 0x1) == 0) ;      /* wait for TimerA timeout flag */
        TIMER0_ICR_R = 0x1;      /* clear the TimerA timeout flag */
    }
}


