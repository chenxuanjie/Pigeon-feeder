#include "stm32f10x.h"                  // Device header
#include "ADC1.h"
#include "main.h"
#include "DMA1.h"
#include "OLED.h"
#include "Rocker.h"

uint8_t RockerNumer;

void Rocker_Init(void)
{
	DMA1_Init();
	ADC1_Init();
}

uint8_t Rocker_GetNum2Loop(void)
{
	/*将物理层X、Y转化为应用层X_App、Y_App*/
	uint8_t Value=0, X=0, Y=0, X_App, Y_App;
	X = (float)ROCKER_X/4096*100;
	Y = (float)ROCKER_Y/4096*100;
	if (X>=40 && X<=59)
	{
		if (Y>=40 && Y<=59)
			return MEDIWM;	
	}
	if (X<=9)	//F4
		Y_App = UP_4;
	else if (X>=10 && X<=19)	//F3
		Y_App = UP_3;
	else if (X>=20 && X<=29)	//F2
		Y_App = UP_2;
	else if (X>=30 && X<=39)	//F1
		Y_App = UP_1;
	else if (X>=60 && X<=69)	//Z1
		Y_App = DOWN_1;
	else if (X>=70 && X<=79)	//Z2
		Y_App = DOWN_2;
	else if (X>=80 && X<=89)	//Z3
		Y_App = DOWN_3;
	else if (X>=90 && X<=99)	//Z4
		Y_App = DOWN_4;
	else Y_App = 0;

	if (Y<=9)	//F4
		X_App = LEFT_4;
	else if (Y>=10 && Y<=19)	//F3
		X_App = LEFT_3;
	else if (Y>=20 && Y<=29)	//F2
		X_App = LEFT_2;
	else if (Y>=30 && Y<=39)	//F1
		X_App = LEFT_1;
	else if (Y>=60 && Y<=69)	//Z1
		X_App = RIGHT_1;
	else if (Y>=70 && Y<=79)	//Z2
		X_App = RIGHT_2;
	else if (Y>=80 && Y<=89)	//Z3
		X_App = RIGHT_3;
	else if (Y>=90 && Y<=99)	//Z4
		X_App = RIGHT_4;	
	else X_App = 0;
	//应用层面的X在左，Y在右
	Value = ((X_App&0x0F)<<4) | (Y_App&0x0F);
	return Value;
}

uint8_t Rocker_GetNum(void)
{
	uint8_t temp;
	temp = RockerNumer;
	RockerNumer = 0;
	return temp;
}


void Rocker_Loop(void)
{
	static uint16_t NowState, LastState;
	LastState = NowState;
	NowState = Rocker_GetNum2Loop();
	if (NowState != LastState)
	{
		RockerNumer = NowState;
	}
}
