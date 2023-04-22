/*
	△接收到的数组及其数值（具体参数都在main.h）
	
	NRF24L01数组元素：
		1.NORMAL_TRANSMIT：常规。
		2.KEY_TRANSMIT：按键。
		3.SWITCH_TRANSMIT：钮子开关。
		4.ROCKER_TRANSMIT：摇杆。
		5.ENCODER_TRANSMIT：旋转编码器。
*/

/*
Program: pigeon-feeder
History:
	2023/4/17	Shane	17th release
	1. change speed() -> Robot_SetSpeed()
*/
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "main.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "TSDA.H"
#include "bsp_usart.h"
#include "LED.H"
#include "ROBOT.H"
#include "Relay.H"
#include "Hcsr04.h"
#include "TIM2.h"
#include "Feed.h"
#include "TrackingLine.h"
#include "State3.h"

int16_t Set_Data,Set_Speed=20, Num;		//Set_Speed 是默认的速度，基于摇杆角度，设定电机按照该速度百分比旋转，单位是cm/s
uint8_t Flag, Hcsr04_StartFlag,LINK_FLAG, Error = 0;
uint8_t X, Y, Value, Feeding_AutoFlag=RESET, Feeding_ManualFlag=RESET, TrackLineFlag=RESET;
int16_t SpeedRight_Robot=0,SpeedLeft_Robot=0;
int8_t LeftCalibration=1,RightCalibration=-1 ;
uint8_t DataReset=1,StopFlag=0;
uint8_t Left=0,Right=0, State=1;
uint32_t Timeout=0;
uint32_t T2Count[3];

machine feeder1, feeder2, feeder3;

void Init(void);
void Data_Analyse(uint8_t *Rocker_XY);
void State1(void);
void State2(void);
void State3(void);
void While_Init(void);
void Remote_Managment(void);
void TrackingLine_Managment(void);
void HandleData(void);
	
int main(void)
{	
	Init();
	while (1)
	{
		While_Init();		
		//循迹模式
		if (TrackLineFlag == SET)
		{
//			TrackLineFlag = !TrackingLine_Test();
			TrackLineFlag = !(TrackingLine(&feeder1, &feeder2, &feeder3));
//			TrackingLine_Managment();
		}
		else
		{
			Remote_Managment();
			switch (State)		//遥控右上角拨杆档位
			{
				case SETTINGSTATE://下
					State3();
					break;
				case CONTROLSTATE://中
					State2();
					break;
				case DEBUGSTATE://上
					State1();
					break;
			}
			/*********************电机驱动************************************/
			SpeedLeft_Robot=Get_LeftDirection()*LeftCalibration*SpeedConversion(Robot_GetSpeedLeft());
			SpeedRight_Robot=Get_RightDirection()*RightCalibration*SpeedConversion(Robot_GetSpeedRight());
			Robot_Move(SpeedLeft_Robot,SpeedRight_Robot);
		}
		HandleData();
		//停止
		if(StopFlag==SET ){Robot_SetSpeed(0);}

		LEDO_OFF();			
	}
}

/**		初始化
  * @brief  Init pigeon-feeder.
  * @param  None
  * @retval None
  */
void Init(void)
{
	TrackingLine_Init();
	LED_Init();
	OLED_Init( );
	NRF24L01_Init();
	RX_Mode();
	Hcsr04_Init();
	TIM2_Init();
	USART_Config();
	Feeding_Init();	
	Robot_Init();
	OLED_printf(3, 1, "1:   2:   3:");
}

void TrackingLine_Managment(void)
{
//	switch(NRF24L01_GetData(NORMAL_TRANSMIT))
//	{
//		case AUTO_FEED_ON:	//自动喂料开
//			Get_FeedTime(&feeder1, &feeder2, &feeder3, Feeding_GetRemoteAutoTimes());
//			Feeding_AutoFlag = SET;
//			Feeding_SetRemoteAutoTimes(Feeding_GetRemoteAutoTimes()+1);
//		break;
//		case AUTO_FEED_OFF:	//自动喂料关
//			Feeding_ResetFeeder(&feeder1, &feeder2, &feeder3); 
//			Feeding_AutoFlag = RESET;
//			Feeding_SetRemoteAutoTimes(0);
//		break;
//	}
//	switch (NRF24L01_GetData(TRACKINGLINE_TRANSMIT))
//	{
//		case TRACKINGLINE_ON:	//自动循迹开启
//			TrackLineFlag = SET;
//		break;
//		case TRACKINGLINE_OFF:
//			TrackLineFlag = RESET;
//		break;
//	}
}

void Remote_Managment(void)
{	
	//启动手动设置时间的落料
	if (Feeding_AutoFlag==RESET && Feeding_ManualFlag==RESET)
		StartFeed(feeder1.Time, feeder2.Time, feeder3.Time);
	else if (Feeding_AutoFlag==SET && Feeding_ManualFlag==RESET)	//启动自动落料
		StartFeed(feeder1.SecondAutoTime_ms, feeder2.SecondAutoTime_ms, feeder3.SecondAutoTime_ms);
	//喂料结束后清0标志位
	if (feeder1.SecondAutoTime_ms==0 && feeder2.SecondAutoTime_ms==0 && feeder3.SecondAutoTime_ms==0)
		Feeding_AutoFlag = RESET;		
}

void While_Init()
{
	OLED_ShowHexNum(1,15,Value,2);  //遥杆坐标
	OLED_ShowNum(2,1,feeder1.SecondAutoTime_ms,4);
	OLED_ShowNum(2,6,feeder2.SecondAutoTime_ms,4);
	OLED_ShowNum(2,11,feeder3.SecondAutoTime_ms,4);
	//定时获取鸽子识别数量
	Get_BirdNum(&Timeout);
	MonitorFeed(&Hcsr04_StartFlag);
	Flag = NRF24L01_ReceiveData();
	//NRF寄存器
	OLED_ShowHexNum(4, 1, NRF24L01_ReadByte(STATUS), 2);
	OLED_ShowHexNum(4, 4, NRF24L01_ReadByte(EN_AA), 2);
	OLED_ShowHexNum(4, 7, NRF24L01_ReadByte(EN_RXADDR), 2);
	OLED_ShowHexNum(4, 10, NRF24L01_ReadByte(RX_ADDR_P0), 2);
	OLED_ShowHexNum(4, 13, NRF24L01_ReadByte(FIFO_STATUS), 2);

	if (NRF24L01_GetData(NORMAL_TRANSMIT) != 0)
	OLED_ShowHexNum(1,1,NRF24L01_GetData(NORMAL_TRANSMIT),2);

	if (Flag)
		Flag = 0;
//	Delay_ms(8);
	Data_Analyse(&Value);	
}

void State1(void)
{
		if(NRF24L01_GetData(SWITCH_TRANSMIT)==SWITCH3_PIN2_NUM+SWITCH2_PIN1_NUM)	//拨钮开关05启用普通按键控制
	{
	/*********************摇杆检测************************************/
		//自旋转弯
//		Robot_SelfTurn(X, Y, Set_Speed);
		//差速转弯
		Robot_DifferentialTurn(X, Y, Set_Speed);
	/********************按键检测************************************/
	
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
//			case EmergencyFault	:	Robot_SetSpeed(0);break;//急停
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//启动
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//关闭
			case KEY_PIN3_NUM		:	Robot_SetSpeed(0);break;//急停
			case KEY_PIN4_NUM		:	break;//使用旋转编码器
			case KEY_PIN5_NUM		:	Set_Speed=NRF24L01_GetData(ENCODER_TRANSMIT);break;
			case KEY_PIN6_NUM		:	break;
			case KEY_PIN7_NUM		:	break;
		
		}

	}
}

void State2(void)
{	
		/* 切换调试模式时，记得按下启动或者停止按键，以免切回普通模式导致保留上一次按键操作*/
	if(NRF24L01_GetData(SWITCH_TRANSMIT)==SWITCH3_PIN2_NUM)//拨钮开关04启用校准模式,校准电机方向，摇杆可用	
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case KEY_PIN4_NUM		:	LeftCalibration=-1;RightCalibration=1;break;
			case KEY_PIN5_NUM		:	LeftCalibration=1;RightCalibration=-1;break;
			case KEY_PIN6_NUM		:	LeftCalibration=1;RightCalibration=1;break;
			case KEY_PIN7_NUM		:	LeftCalibration=-1;RightCalibration=-1;break;
		}
		
		
//		/*********************旋转编码器检测************************************/
//	
//		if(NRF24L01_GetData(KEY_TRANSMIT)==KEY_PIN4_NUM)
//		{
//			Set_Data=NRF24L01_GetData(ENCODER_TRANSMIT);
//			Robot_SetSpeed(Set_Data);
//		}

	
/*********************喂料与循迹************************************/		
		switch( NRF24L01_GetData(NORMAL_TRANSMIT))
		{
			case FEED_OFF:	//喂料器关
				if(Feeding_AutoFlag == RESET)
					Relay_Set(ALL, RESET);
				Feeding_ManualFlag = RESET;
			break;
			case FEED_ON:	//喂料器开
				Relay_Set(ALL, SET);
				Feeding_ManualFlag = SET;
			break;
			case AUTO_FEED_ON:	//自动喂料开
				Get_FeedTime(&feeder1, &feeder2, &feeder3, Feeding_GetRemoteAutoTimes());
				Feeding_AutoFlag = SET;
				Feeding_SetRemoteAutoTimes(Feeding_GetRemoteAutoTimes()+1);
			break;
			case AUTO_FEED_OFF:	//自动喂料关(若关闭，需要回退一格笼子重新识别当前要喂料的笼子)
				Feeding_ResetFeeder(&feeder1, &feeder2, &feeder3); 
				Feeding_AutoFlag = RESET;
				Feeding_SetRemoteAutoTimes(0);
			break;
		}
		switch (NRF24L01_GetData(TRACKINGLINE_TRANSMIT))
		{
			case TRACKINGLINE_ON:	//自动循迹开启
				TrackLineFlag = SET;
			break;
			case TRACKINGLINE_OFF:
				TrackLineFlag = RESET;
			break;
		}
		
		
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault	:	Robot_SetSpeed(0);break;//急停
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//启动
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//关闭
			case KEY_PIN3_NUM		:	Robot_SetSpeed(0);break;//急停
		}
		
		
	}

void State3(void)
{
	switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault		:	Robot_SetSpeed(0);break;//急停
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//启动
			case KEY_PIN2_NUM		:	break;
			case KEY_PIN3_NUM		:	Robot_SetSpeed(0);break;//急停
			case KEY_PIN4_NUM		:	break;
			case KEY_PIN5_NUM		:	break;
			case KEY_PIN6_NUM		:	break;
			case KEY_PIN7_NUM		:	break;
		}
}

void Data_Analyse(uint8_t *Rocker_XY)
{
	static uint8_t SwitchNum, LastSwitchNum;	
	*Rocker_XY = NRF24L01_GetData(ROCKER_TRANSMIT);
	//X表示实际操作时摇杆左、右拨(应用层)
	X = (Value&0xF0)>>4;
	Y = Value&0x0F;
	
	LastSwitchNum = SwitchNum;
	//State值的改变
	SwitchNum = NRF24L01_GetData(SWITCH_TRANSMIT);	
	if (SwitchNum != LastSwitchNum)
	{
		if ((SwitchNum&0x02) != 0)	//三挡钮子开关向下拨
		{
			State = SETTINGSTATE;		
		}
		else if ((SwitchNum&0x08) != 0)	//三挡钮子开关向中间拨
		{
			State = CONTROLSTATE;
		}
		else 							//三挡钮子开关向上拨
			State = DEBUGSTATE;
	}
}

void HandleData(void)
{
	static uint8_t NormalNum, LastLinkFlag, NowLinkFlag;
	NormalNum = NRF24L01_GetData(NORMAL_TRANSMIT);

	//断开连接的情况
	LastLinkFlag = NowLinkFlag;
	NowLinkFlag = LINK_FLAG;
	if (LastLinkFlag==0 && NowLinkFlag==1)
	StopFlag=RESET;
	else if (LastLinkFlag==1 && NowLinkFlag==0) //通讯断开
	{StopFlag=SET;
	}
	//常规发送位最高位作为连接标志位	
	if ((NormalNum&0x80)!=RESET && Error==SET)	//半秒后最高位没有被清除
	{
		LINK_FLAG = 0;
	}
	else if ((NormalNum&0x80)==RESET && Error==SET)//正常工作
	{

		LINK_FLAG = 1;
		Error = RESET;		//不是错误情况
		T2Count[2] = 0;		
		NRF24L01_SetRXBuf(NORMAL_TRANSMIT, (NormalNum |= 0x80));	//置位标志位，看下次是否被清除

	}
	
}

void TIM2_IRQHandler(void)
{

	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
	{
		//0.5s无响应，则断开连接
		T2Count[2] ++;
		if (T2Count[2] >= 500)
		{
			Error = SET;
		}
		else
			Error = RESET;
		
		T2Count[0]++;
		if (T2Count[0] >= 100)
		{
			T2Count[0] = 0;
			if ((Hcsr04_StartFlag&0x07) < 4)	//小于4，加
				Hcsr04_StartFlag ++;
			else 
				Hcsr04_StartFlag = 0x01;		//大于4，变为1
			Hcsr04_StartFlag |= 0x08;			//超声波检测使能位
		}

        //Timeout减到0，则获取鸽子数量
        if (Timeout > 0) 
			Timeout --;
		//喂料器时间
		if (feeder1.Time > 0) feeder1.Time --;
		if (feeder2.Time > 0) feeder2.Time --;
		if (feeder3.Time > 0) feeder3.Time --;
		//自动落料的时间
		if (feeder1.SecondAutoTime_ms > 0) feeder1.SecondAutoTime_ms --;
		if (feeder2.SecondAutoTime_ms > 0) feeder2.SecondAutoTime_ms --;
		if (feeder3.SecondAutoTime_ms > 0) feeder3.SecondAutoTime_ms --;
		//循迹模式下的延时时间
		if (Robot_DelayGet() != 0)
			Robot_DelaySet(Robot_DelayGet()-1);
	}
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	
}
