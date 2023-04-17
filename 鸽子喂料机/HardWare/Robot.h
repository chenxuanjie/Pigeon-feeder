#ifndef __ROBOT_H_
#define __ROBOT_H_

#include "stm32f10x.h"


#define ReductionRatio 			40	//	������ٱ� 1��40
#define TireDiameter 				250 //�����⾶250mm
#define LeftWheel		  			TSDA_Usart  //����
#define RightWheel					TSDA_Usart2 //�ҳ���

void Robot_Init(void);
void Robot_SetSpeed(int16_t data);
int16_t Robot_GetSpeedLeft(void);
int16_t Robot_GetSpeedRight(void);
void Robot_SetSpeedLeft(int16_t Data);
void Robot_SetSpeedRight(int16_t Data);
int16_t SpeedConversion(int32_t data);
void Robot_Move(int16_t LeftSpeed,int16_t RightSpeed);
void Robot_ShutDown(void);
#endif
