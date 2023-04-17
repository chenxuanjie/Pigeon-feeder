#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "ROBOT.H"


//×ÔÐý×ªÍä
void Robot_SelfTurn(uint8_t X, uint8_t Y, int8_t* LeftInversion, int8_t* RightInversion, int16_t Set_Speed)
{
	switch(Y)
	{
		case STOP_Y:Robot_SetSpeed(0x00);break;
		case UP_1:Robot_SetSpeed(Set_Speed/10);break;
		case UP_2:Robot_SetSpeed(Set_Speed/4);break;
		case UP_3:Robot_SetSpeed(Set_Speed/2);break;
		case UP_4:Robot_SetSpeed(Set_Speed);break;
		case DOWN_1:Robot_SetSpeed(-Set_Speed/10);break;
		case DOWN_2:Robot_SetSpeed(-Set_Speed/4);break;
		case DOWN_3:Robot_SetSpeed(-Set_Speed/2);break;
		case DOWN_4:Robot_SetSpeed(-Set_Speed);break;
	}
	*LeftInversion=1;*RightInversion=1;
	switch(X)
	{			
		case LEFT_1:break;
		case LEFT_2:Robot_SetSpeed(Set_Speed/10);*LeftInversion=-1;break;
		case LEFT_3:Robot_SetSpeed(Set_Speed/4);*LeftInversion=-1;break;
		case LEFT_4:Robot_SetSpeed(Set_Speed/2);*LeftInversion=-1;break;
		case RIGHT_1:break;
		case RIGHT_2:Robot_SetSpeed(Set_Speed/10);*RightInversion=-1;break;
		case RIGHT_3:Robot_SetSpeed(Set_Speed/4);*RightInversion=-1;break;
		case RIGHT_4:Robot_SetSpeed(Set_Speed/2);*RightInversion=-1;break;
		case STOP_X:Robot_SetSpeed(0x00);break;	
	}

}
