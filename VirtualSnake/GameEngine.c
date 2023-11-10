
/*struct GameObject {
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
};         
typedef struct GameObject GTyp;
*/
/*
#include "GameEngine.h"
#include "Nokia5110.h"
#include "Random.h"

// This file keeps track of data

// Maximum dimensions of the LCD
#define BORDER_X				84
#define BORDER_Y				48
#define START_X					BORDER_X/2
#define START_Y					BORDER_Y/2


struct GameObject {
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
};         
typedef struct GameObject GTyp;

/*
struct SnakeSprite {
	GTyp GObj;
	const unsigned char *img;
};
typedef struct SnakeSprite STyp;

struct FruitSprite {
	GTyp GObj;
	const unsigned char *img;
};
typedef struct FruitSprite FTyp;


// =====================================================================================

//static GTyp snake_head;


void Draw_Border(void){
	int i, j;
	//Nokia5110_ClearBuffer();
  //Nokia5110_Clear();

	for(i = 0; i < BORDER_Y; i++){
		
		for(j = 0; j < BORDER_X; j++){
			
			if((i == 0) || (i == BORDER_Y - 1) || (j == 0) || (j == BORDER_X - 1))
				Nokia5110_SetPixel(j,i);
			else 
				continue;
		}
	}
  
	//Nokia5110_SetCursor(3, 1);
	
  //Nokia5110_OutString("Test");	
	//Nokia5110_SetCursor(2, 2);
	//Nokia5110_OutString("Drawn");	

}

*/