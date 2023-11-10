#ifndef INIT_H_
#define INIT_H_

//Initializes the SysTick timer in main()
void SysTick_Init(unsigned long period); 

//Initializes PORTF pins in main()
void PortF_Init(void);

//Initializes PORTE pins in main()
void PortE_Init(void);

#endif
