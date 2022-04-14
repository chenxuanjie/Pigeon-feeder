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
	/*以实际遥控的物理位置标定xy*/
	uint8_t Value=0, X=0, Y=0;
	Y = (float)ROCKER_X/4096*100;
	X = (float)ROCKER_Y/4096*100;
//	OLED_ShowNum(1, 3, X, 2);
//	OLED_ShowNum(1, 8, Y, 2);
	if (X>=40 && X<=59)
	{
		if (Y>=40 && Y<=59)
			return MEDIWM;	
	}
	if (X<=9)
		X = LEFT_4;
	else if (X>=10 && X<=19)
		X = LEFT_3;
	else if (X>=20 && X<=29)
		X = LEFT_2;
	else if (X>=30 && X<=39)
		X = LEFT_1;
	else if (X>=60 && X<=69)
		X = RIGHT_1;
	else if (X>=70 && X<=79)
		X = RIGHT_2;
	else if (X>=80 && X<=89)
		X = RIGHT_3;
	else if (X>=90 && X<=99)
		X = RIGHT_4;
	else X = 0;

	if (Y<=9)
		Y = UP_4;
	else if (Y>=10 && Y<=19)
		Y = UP_3;
	else if (Y>=20 && Y<=29)
		Y = UP_2;
	else if (Y>=30 && Y<=39)
		Y = UP_1;
	else if (Y>=60 && Y<=69)
		Y = DOWN_1;
	else if (Y>=70 && Y<=79)
		Y = DOWN_2;
	else if (Y>=80 && Y<=89)
		Y = DOWN_3;
	else if (Y>=90 && Y<=99)
		Y = DOWN_4;	
	else Y = 0;
	
	Value = (X&0x0F) | ((Y&0x0F)<<4);
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
