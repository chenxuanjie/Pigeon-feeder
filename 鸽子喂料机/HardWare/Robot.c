#include "ROBOT.H"
#include "TSDA.H"
#include "DELAY.H"
#include "TrackingLine.h"
#include "NRF24L01.h"
#include "main.h"
#include "OLED.h"

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
	return -Direction_Right;
}


/**		С����ʻ���ٶȵȼ���
  * @brief   С����ʻ���ٶȵȼ���
  * @param  ��
  * @retval ��
  */
uint8_t Robot_GetSpeedLevel(void)
{
	return NRF24L01_GetData(ENCODER_TRANSMIT);
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
	Robot_SetSpeedLeft(TRACKINGLINE_FRONTSPEED);
	Robot_SetSpeedRight(TRACKINGLINE_FRONTSPEED + TRACKINGLINE_TURNSPEED);
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
	Robot_SetSpeedLeft(TRACKINGLINE_FRONTSPEED + TRACKINGLINE_TURNSPEED);
	Robot_SetSpeedRight(TRACKINGLINE_FRONTSPEED);
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
	Set_LeftDirection(FRONT);
	Set_RightDirection(FRONT);
	Robot_SetSpeedLeft(TRACKINGLINE_FRONTSPEED);
	Robot_SetSpeedRight(TRACKINGLINE_FRONTSPEED);
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
	Robot_Start();
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
	Robot_SetSpeedLeft(TRACKINGLINE_SELFTURNSPEED);
	Robot_SetSpeedRight(TRACKINGLINE_SELFTURNSPEED);	
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
	Robot_Start();
	while (Robot_DelayGet())
	{
		if (TrackingLine_IfExit())
			return 1;
		Robot_Front();
		OLED_ShowNum(2,16,1,1);
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
		TSDA_Data(LeftWheel,SpeedSetting,0X00);
		TSDA_Data(RightWheel,SpeedSetting,0X00);
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
	//����ͨѶ�ж��Զ�ͣ��
	TSDA_Order(LeftWheel,CommunicationOutage_ON);
	TSDA_Order(RightWheel,CommunicationOutage_ON);
	Delay_ms(10);
	//ѡ���ٶ�ģʽ����
	TSDA_Order(LeftWheel,SpeedMod);
	TSDA_Order(RightWheel,SpeedMod);
	//�趨�ӡ����ٶ�ʱ��, 2000ms
	TSDA_Data(LeftWheel, A_D_Time, 0x64);
	TSDA_Data(RightWheel, A_D_Time, 0x64);
	TSDA_Order(LeftWheel,MotorStop);
	TSDA_Order(RightWheel,MotorStop);
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
  * @brief  ���м��ٶȵ��ƶ�
* @param  LeftSpeed:�����ٶȣ�RightSpeed�������ٶȣ�
  * @retval ��
  */
//uint8_t Robot_Move2(int16_t LeftSpeed,int16_t RightSpeed)
//{
//	int16_t MaxSpeed, StartSpeed_Left, StartSpeed_Right;
//	StartSpeed_Left = Robot_GetSpeedLeft();
//	StartSpeed_Right = Robot_GetSpeedRight();
//	//����ٶ�
//	if (LeftSpeed > RightSpeed)
//		MaxSpeed = LeftSpeed;
//	else
//		MaxSpeed = RightSpeed;	
//	while ()					//ѭ������
//	{
//		//�Ƿ��˳�ѭ��ģʽ
//		if (TrackingLine_IfExit())
//			return 1;
//		//����ٶ�δ�ﵽ�����ٶ���
//		if (i < Robot_GetSpeedLeft())
//			 Robot_SetSpeedLeft(Robot_GetSpeedLeft()+1);
//		if (i < Robot_GetSpeedRight())
//			 Robot_SetSpeedRight(Robot_GetSpeedRight()+1);
//		Robot_DelaySet(Time/(100-Value));	//ADDSPEED_TIME ms����ɼ���
//		while (Robot_DelayGet()&& Crossload_Flag)
//			else
//				Digital_Straight();		//����ʶ����ߡ�
//			if (Digital_L1==1 && Digital_R1==0)	//΢΢ƫ��
//				Robot_Move();
//			else if (Digital_L1==0 && Digital_M==1 && Digital_R1==1)	//΢΢ƫ��
//			{
//				Robot_TurnRight(FRONT);
//			}
//	}
//	return 0;	
//}

/**
  * @brief  �������
  * @param  ��
  * @retval ��
  */
void Robot_Start(void)
{
	TSDA_Order(LeftWheel,MotorStart);
	TSDA_Order(RightWheel,MotorStart);
}

/**
  * @brief  ����ͣ�������������ٶ�Ϊ0��ֹͣ����ʹ���������������
  * @param  ��
  * @retval ��
  */
void Robot_Stop(void)
{
	Robot_SetSpeed(0);
	TSDA_Order(LeftWheel,MotorStop);
	TSDA_Order(RightWheel,MotorStop);	
}
