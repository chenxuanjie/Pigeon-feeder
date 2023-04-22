#include "ROBOT.H"
#include "TSDA.H"
#include "DELAY.H"
#include "TrackingLine.h"
#include "NRF24L01.h"
#include "main.h"

uint16_t Delay_Time;
int8_t Direction_Right = FRONT, Direction_Left = FRONT;
int16_t SpeedLeft=0,SpeedRight=0;

void Set_LeftDirection(int8_t Direction)
{
	Direction_Left = Direction;
}

void Set_RightDirection(int8_t Direction)
{
	Direction_Right = Direction;
}

int8_t Get_LeftDirection(void)
{
	return Direction_Left;
}

int8_t Get_RightDirection(void)
{
	return Direction_Left;
}

/**		ѭ��ģʽ�µ���ת��
  * @brief  Turn left in the TrackingLine mode. 
  * @param  ��
  * @retval ��
  */
void Robot_TurnLeft(uint8_t Direction)
{
	//ȷ��ǰ�����ߺ���
	Set_LeftDirection(Direction); 
	Set_RightDirection(Direction); 
	//�趨�ٶ�	
	Robot_SetSpeedLeft(TRACKINGLINE_BASESPEED + TRACKINGLINE_TURNSPEED);
	Robot_SetSpeedRight(TRACKINGLINE_BASESPEED);
	Robot_Move(Get_LeftDirection() * SpeedConversion(Robot_GetSpeedLeft()),\
				Get_RightDirection() * SpeedConversion(Robot_GetSpeedRight()));
}

/**		ѭ��ģʽ�µ���ת��
  * @brief  Turn right in the TrackingLine mode. 
  * @param  ��
  * @retval ��
  */
void Robot_TurnRight(uint8_t Direction)
{
	//ȷ��ǰ�����ߺ���
	Set_LeftDirection(Direction); 
	Set_RightDirection(Direction); 
	//�趨�ٶ�	
	Robot_SetSpeedLeft(TRACKINGLINE_BASESPEED);
	Robot_SetSpeedRight(TRACKINGLINE_BASESPEED + TRACKINGLINE_TURNSPEED);
	Robot_Move(Get_LeftDirection() * SpeedConversion(Robot_GetSpeedLeft()),\
				Get_RightDirection() * SpeedConversion(Robot_GetSpeedRight()));
}

/**		ѭ��ģʽ�µ�ֱ�С�
  * @brief  Go straight in the TrackingLine mode. 
  * @param  ��
  * @retval ��
  */
void Robot_Front(void)
{
	Robot_SetSpeedLeft(TRACKINGLINE_BASESPEED);
	Robot_SetSpeedRight(TRACKINGLINE_BASESPEED);
	Robot_Move(Get_LeftDirection() * SpeedConversion(Robot_GetSpeedLeft()),\
				Get_RightDirection() * SpeedConversion(Robot_GetSpeedRight()));
}

/**		ѭ��ģʽ�µ�������ת��(DirectionΪ˳ʱ�����ʱ��)
  * @brief  Self Turn in the TrackingLine mode. 
* @param  Direction: SHUN: 
					 NI:
  * @retval ��
  */
void Robot_Cirle(uint8_t Direction)
{
	if (Direction == SHUN)
	{
		Direction_Left = FRONT;		
		Direction_Right = BACK;		//��ת�����ֺ��ˡ�
	}		
	else if (Direction == NI)
	{
		Direction_Left = BACK;		//��ת�����ֺ��ˡ�		
		Direction_Right = FRONT;				
	}
	Robot_SetSpeedLeft(TRACKINGLINE_BASESPEED);
	Robot_SetSpeedRight(TRACKINGLINE_BASESPEED);	
	Robot_Move(Get_LeftDirection() * SpeedConversion(Robot_GetSpeedLeft()),\
				Get_RightDirection() * SpeedConversion(Robot_GetSpeedRight()));
}

/**
  * @brief  ͨ���趨����ʱʱ�䣬��ȡ��ǰʣ��ʱ��ֵ��
  * @param  ��
  * @retval ʣ���ʱ��ֵ����λ��ms
  */
uint16_t Robot_DelayGet(void)
{
	return Delay_Time;
}

/**
  * @brief  �趨��ʱʱ�䣬����1��Ϊ1ms��
  * @param  Num����ʱValue ms��
  * @retval ��
  */
void Robot_DelaySet(uint16_t Num)
{
		Delay_Time = Num;
}

/**
  * @brief  ����С��ֱ��ʱ�䡣
  * @param  Num:����ʱ�䡣��λ��ms
* @retval 1: unexpected return. exit the trackingline mode.
		  0: end go straight. 
  */
uint8_t Robot_RunTime(uint16_t Num)
{
	Robot_DelaySet(Num);
	while (Robot_DelayGet())
	{
		if (TrackingLine_IfExit())
			return 1;
		else		
			Digital_Straight();
	}
	return 0;
}

/**
  * @brief  ����С��ֹͣʱ�䡣
  * @param  Num:ֹͣʱ�䡣��λ��ms
* @retval 1: unexpected return. exit the trackingline mode.
		  0: end stop. 
  */
uint8_t Robot_StopTime(uint16_t Num)
{
	Robot_DelaySet(Num);
	while(Robot_DelayGet())
	{
		Robot_Stop();
		if (TrackingLine_IfExit())
			return 1;
	}
	return 0;
}

void Robot_SetSpeed(int16_t data)
{
	SpeedLeft=data;
	SpeedRight=data;
}

void Robot_SetSpeedLeft(int16_t Data)
{
	SpeedLeft = Data;
}

void Robot_SetSpeedRight(int16_t Data)
{
	SpeedRight = Data;
}

int16_t Robot_GetSpeedLeft(void)
{
	return SpeedLeft;
}

int16_t Robot_GetSpeedRight(void)
{
	return SpeedRight;
}

/**
  * @brief  ��λ���㣨cm/s -> ͨѶ���ݣ�
  * @param  data:ι�ϻ��ƶ��ٶ� ��λcm/s ֻ��������0~100��
  * @retval ��
  */
int16_t SpeedConversion(int32_t data)
{
	if(data==0);
	else if(data>=-100&&data<=100)
	{
	data=data*60*ReductionRatio;		 //r/s����ΪRPM
	data=data*10/TireDiameter/3.14;  //cm/s����Ϊr/s
	data=data*8192/3000;						 //RPM����Ϊ�ֽ�����
	
	}
	else data=0;					 //���޻���Ϊ3000RPM
	return (int16_t)data;
}

/**
  * @brief  ��ʼ������ι�ϻ�
  * @param  ��
  * @retval ��
  */
void Robot_Init(void)
{
	TSDA_Order(LeftWheel,MotorStop);
	TSDA_Order(RightWheel,MotorStop);
	//����ͨѶ�ж��Զ�ͣ��
	TSDA_Order(LeftWheel,CommunicationOutage_ON);
	TSDA_Order(RightWheel,CommunicationOutage_ON);
	Delay_ms(10);
	//ѡ���ٶ�ģʽ����
	TSDA_Order(LeftWheel,SpeedMod);
	TSDA_Order(RightWheel,SpeedMod);
	Delay_ms(10);	
	
}

/**
  * @brief  ι�ϻ��ƶ�����
* @param  LeftSpeed:�����ٶȣ�RightSpeed�������ٶȣ�
  * @retval ��
  */
void Robot_Move(int16_t LeftSpeed,int16_t RightSpeed)
{
	TSDA_Data(LeftWheel,SpeedSetting,LeftSpeed);
	TSDA_Data(RightWheel,SpeedSetting,RightSpeed);
	
}
/**
  * @brief  ����ͣ��
  * @param  ��
  * @retval ��
  */
void Robot_Stop(void)
{
	Robot_SetSpeed(0);
	TSDA_Data(LeftWheel,SpeedSetting,0X00);
	TSDA_Data(RightWheel,SpeedSetting,0X00);
}
