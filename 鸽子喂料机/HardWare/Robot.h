#ifndef __ROBOT_H_
#define __ROBOT_H_

#include "stm32f10x.h"

#define ReductionRatio 			40	//	电机减速比 1：40
#define TireDiameter 				250 //车轮外径250mm
#define LeftWheel		  			TSDA_Usart  //左车轮
#define RightWheel					TSDA_Usart2 //右车轮

void Set_LeftDirection(int8_t Direction);
void Set_RightDirection(int8_t Direction);
int8_t Get_LeftDirection(void);
int8_t Get_RightDirection(void);
void Robot_Init(void);
void Robot_SetSpeed(int16_t data);
int16_t Robot_GetSpeedLeft(void);
int16_t Robot_GetSpeedRight(void);
void Robot_SetSpeedLeft(int16_t Data);
void Robot_SetSpeedRight(int16_t Data);
int16_t SpeedConversion(int32_t data);
void Robot_Move(int16_t LeftSpeed,int16_t RightSpeed);
void Robot_Stop(void);
void Robot_TurnLeft(uint8_t Direction);
void Robot_TurnRight(uint8_t Direction);
void Robot_Front(void);
void Robot_Cirle(uint8_t Direction);
uint8_t Robot_RunTime(uint16_t Num);
uint8_t Robot_StopTime(uint16_t Num);
void Robot_DelaySet(uint16_t Num);
uint16_t Robot_DelayGet(void);

#endif
