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


/**		小车行驶的速度等级。
  * @brief   小车行驶的速度等级。
  * @param  无
  * @retval 无
  */
uint8_t Robot_GetSpeedLevel(void)
{
	return NRF24L01_GetData(ENCODER_TRANSMIT);
}

/**		循迹模式下的左转。
  * @brief  Turn left in the TrackingLine mode. 
  * @param  无
  * @retval 无
  */
void Robot_TurnLeft(uint8_t Direction)
{
	//确定前进或者后退
	Set_LeftDirection(Direction); 
	Set_RightDirection(Direction); 
	//设定速度	
	Robot_SetSpeedLeft(TRACKINGLINE_FRONTSPEED);
	Robot_SetSpeedRight(TRACKINGLINE_FRONTSPEED + TRACKINGLINE_TURNSPEED);
	Robot_Move(Get_LeftDirection() * SpeedConversion(Robot_GetSpeedLeft()),\
				Get_RightDirection() * SpeedConversion(Robot_GetSpeedRight()));
}

/**		循迹模式下的右转。
  * @brief  Turn right in the TrackingLine mode. 
  * @param  无
  * @retval 无
  */
void Robot_TurnRight(uint8_t Direction)
{
	//确定前进或者后退
	Set_LeftDirection(Direction); 
	Set_RightDirection(Direction); 
	//设定速度	
	Robot_SetSpeedLeft(TRACKINGLINE_FRONTSPEED + TRACKINGLINE_TURNSPEED);
	Robot_SetSpeedRight(TRACKINGLINE_FRONTSPEED);
	Robot_Move(Get_LeftDirection() * SpeedConversion(Robot_GetSpeedLeft()),\
				Get_RightDirection() * SpeedConversion(Robot_GetSpeedRight()));
}

/**		循迹模式下的直行。
  * @brief  Go straight in the TrackingLine mode. 
  * @param  无
  * @retval 无
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

/**		循迹模式下的自旋旋转。(Direction为顺时针或逆时针)
  * @brief  Self Turn in the TrackingLine mode. 
* @param  Direction: SHUN: 
					 NI:
  * @retval 无
  */
void Robot_Cirle(uint8_t Direction)
{
	Robot_Start();
	if (Direction == SHUN)
	{
		Direction_Left = FRONT;		
		Direction_Right = BACK;		//右转，右轮后退。
	}		
	else if (Direction == NI)
	{
		Direction_Left = BACK;		//左转，左轮后退。		
		Direction_Right = FRONT;				
	}
	Robot_SetSpeedLeft(TRACKINGLINE_SELFTURNSPEED);
	Robot_SetSpeedRight(TRACKINGLINE_SELFTURNSPEED);	
	Robot_Move(Get_LeftDirection() * SpeedConversion(Robot_GetSpeedLeft()),\
				Get_RightDirection() * SpeedConversion(Robot_GetSpeedRight()));
}

/**
  * @brief  通过设定的延时时间，获取当前剩余时间值。
  * @param  无
  * @retval 剩余的时间值。单位：ms
  */
uint16_t Robot_DelayGet(void)
{
	return Delay_Time;
}

/**
  * @brief  设定延时时间，设置1则为1ms。
  * @param  Num：延时Value ms。
  * @retval 无
  */
void Robot_DelaySet(uint16_t Num)
{
		Delay_Time = Num;
}

/**
  * @brief  设置小车直走时间。
  * @param  Num:运行时间。单位：ms
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
  * @brief  设置小车停止时间。
  * @param  Num:停止时间。单位：ms
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
  * @brief  单位换算（cm/s -> 通讯数据）
  * @param  data:喂料机移动速度 单位cm/s 只容许输入0~100；
  * @retval 无
  */
int16_t SpeedConversion(int32_t data)
{
	if(data==0);
	else if(data>=-100&&data<=100)
	{
	data=data*60*ReductionRatio;		 //r/s换算为RPM
	data=data*10/TireDiameter/3.14;  //cm/s换算为r/s
	data=data*8192/3000;						 //RPM换算为字节数据
	
	}
	else data=0;					 //超限换算为3000RPM
	return (int16_t)data;
}

/**
  * @brief  初始化鸽子喂料机
  * @param  无
  * @retval 无
  */
void Robot_Init(void)
{
	//开启通讯中断自动停机
	TSDA_Order(LeftWheel,CommunicationOutage_ON);
	TSDA_Order(RightWheel,CommunicationOutage_ON);
	Delay_ms(10);
	//选择速度模式输入
	TSDA_Order(LeftWheel,SpeedMod);
	TSDA_Order(RightWheel,SpeedMod);
	//设定加、减速度时间, 2000ms
	TSDA_Data(LeftWheel, A_D_Time, 0x64);
	TSDA_Data(RightWheel, A_D_Time, 0x64);
	TSDA_Order(LeftWheel,MotorStop);
	TSDA_Order(RightWheel,MotorStop);
	Delay_ms(10);	
	
}

/**
  * @brief  喂料机移动参数
* @param  LeftSpeed:左轮速度；RightSpeed：右轮速度；
  * @retval 无
  */
void Robot_Move(int16_t LeftSpeed,int16_t RightSpeed)
{
	TSDA_Data(LeftWheel,SpeedSetting,LeftSpeed);
	TSDA_Data(RightWheel,SpeedSetting,RightSpeed);	
}

/**
  * @brief  带有加速度的移动
* @param  LeftSpeed:左轮速度；RightSpeed：右轮速度；
  * @retval 无
  */
//uint8_t Robot_Move2(int16_t LeftSpeed,int16_t RightSpeed)
//{
//	int16_t MaxSpeed, StartSpeed_Left, StartSpeed_Right;
//	StartSpeed_Left = Robot_GetSpeedLeft();
//	StartSpeed_Right = Robot_GetSpeedRight();
//	//最大速度
//	if (LeftSpeed > RightSpeed)
//		MaxSpeed = LeftSpeed;
//	else
//		MaxSpeed = RightSpeed;	
//	while ()					//循环加速
//	{
//		//是否退出循迹模式
//		if (TrackingLine_IfExit())
//			return 1;
//		//如果速度未达到，则速度增
//		if (i < Robot_GetSpeedLeft())
//			 Robot_SetSpeedLeft(Robot_GetSpeedLeft()+1);
//		if (i < Robot_GetSpeedRight())
//			 Robot_SetSpeedRight(Robot_GetSpeedRight()+1);
//		Robot_DelaySet(Time/(100-Value));	//ADDSPEED_TIME ms内完成加速
//		while (Robot_DelayGet()&& Crossload_Flag)
//			else
//				Digital_Straight();		//车子识别黑线。
//			if (Digital_L1==1 && Digital_R1==0)	//微微偏右
//				Robot_Move();
//			else if (Digital_L1==0 && Digital_M==1 && Digital_R1==1)	//微微偏左
//			{
//				Robot_TurnRight(FRONT);
//			}
//	}
//	return 0;	
//}

/**
  * @brief  电机启动
  * @param  无
  * @retval 无
  */
void Robot_Start(void)
{
	TSDA_Order(LeftWheel,MotorStart);
	TSDA_Order(RightWheel,MotorStart);
}

/**
  * @brief  紧急停机。不是设置速度为0的停止（会使电机产生轰鸣声）
  * @param  无
  * @retval 无
  */
void Robot_Stop(void)
{
	Robot_SetSpeed(0);
	TSDA_Order(LeftWheel,MotorStop);
	TSDA_Order(RightWheel,MotorStop);	
}
