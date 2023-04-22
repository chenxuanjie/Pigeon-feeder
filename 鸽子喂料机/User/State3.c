#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "ROBOT.H"
#include "State3.h"
#include "OLED.h"
#include "NRF24L01.h"

/**		差速转弯
  * @brief  Run by a differential way.
* @param  X: 
  		  Y: 
		  Set_Speed: the normal speed, often 20. 
  * @retval None
  */
void Robot_DifferentialTurn(uint8_t X, uint8_t Y, int16_t Set_Speed)
{
	int16_t FinalSPeedLeft=0, FinalSPeedRight=0;
	//speed/10 speed/4 speed/2 speed
	// add speed with TURNSPEED
	switch(Y)
	{
		case UP_1: FinalSPeedLeft += SPEED1;  FinalSPeedRight += SPEED1;break;	//改善手感，在摇杆1处不改变方向
		case UP_2: Set_LeftDirection(FRONT);Set_RightDirection(FRONT);
			FinalSPeedLeft += SPEED1;  FinalSPeedRight += SPEED1;break;
		case UP_3: Set_LeftDirection(FRONT);Set_RightDirection(FRONT);
			FinalSPeedLeft += SPEED2;  FinalSPeedRight += SPEED2;break;
		case UP_4:  Set_LeftDirection(FRONT);Set_RightDirection(FRONT);
			FinalSPeedLeft += SPEED3;  FinalSPeedRight += SPEED3;break;
		case DOWN_1: FinalSPeedLeft += SPEED1;  FinalSPeedRight += SPEED1;break;	//改善手感，在摇杆1处不改变方向
		case DOWN_2: Set_LeftDirection(BACK);Set_RightDirection(BACK);
			FinalSPeedLeft += SPEED1;  FinalSPeedRight += SPEED1;break;
		case DOWN_3: Set_LeftDirection(BACK);Set_RightDirection(BACK);
			FinalSPeedLeft += SPEED2;  FinalSPeedRight += SPEED2;break;
		case DOWN_4: Set_LeftDirection(BACK);Set_RightDirection(BACK);
			FinalSPeedLeft += SPEED3;  FinalSPeedRight += SPEED3;break;
		case STOP_Y: Robot_SetSpeed(0x00);break;
	}
	switch(X)
	{			
		case LEFT_1:FinalSPeedLeft += TURNSPEED/2;FinalSPeedRight += TURNSPEED;break;
		case LEFT_2:FinalSPeedRight += TURNSPEED;break;
		case LEFT_3:FinalSPeedRight += TURNSPEED;break;
		case LEFT_4:FinalSPeedRight += TURNSPEED;;break;
		case RIGHT_1:FinalSPeedLeft += TURNSPEED; FinalSPeedLeft += TURNSPEED/2;break;
		case RIGHT_2: FinalSPeedLeft += TURNSPEED;break;
		case RIGHT_3: FinalSPeedLeft += TURNSPEED;break;
		case RIGHT_4: FinalSPeedLeft += TURNSPEED;break;
		case STOP_X:Robot_SetSpeed(0x00);break;	
	}
	//在摇杆负方向回正后拨平，车体应该向前。改善手感。
	if (X==STOP_X && Y==STOP_Y)
	{
		Set_LeftDirection(FRONT);
		Set_RightDirection(FRONT);
	}
	//摇杆不在中位时才可设置速度。(避免摇杆在中位时仍然会动)
	if (Y!=STOP_Y || X!=STOP_X)
	{
		Robot_SetSpeedLeft(FinalSPeedLeft);
		Robot_SetSpeedRight(FinalSPeedRight);	
	}

}

//自旋转弯
void Robot_SelfTurn(uint8_t X, uint8_t Y, int16_t Set_Speed)
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
	Set_LeftDirection(FRONT);
	Set_RightDirection(FRONT);	
	switch(X)
	{			
		case LEFT_1:break;
		case LEFT_2:Robot_SetSpeed(Set_Speed/10);Set_LeftDirection(BACK);break;
		case LEFT_3:Robot_SetSpeed(Set_Speed/4);Set_LeftDirection(BACK);break;
		case LEFT_4:Robot_SetSpeed(Set_Speed/2);Set_LeftDirection(BACK);break;
		case RIGHT_1:break;
		case RIGHT_2:Robot_SetSpeed(Set_Speed/10);Set_RightDirection(BACK);;break;
		case RIGHT_3:Robot_SetSpeed(Set_Speed/4);Set_RightDirection(BACK);break;
		case RIGHT_4:Robot_SetSpeed(Set_Speed/2);Set_RightDirection(BACK);break;
		case STOP_X:Robot_SetSpeed(0x00);break;	
	}

}
