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
	2023/4/16	Shane	16th release
	1. add some definition of remote control. 代码重构
	2. delete LED1,LED2
	3. change logic of handling distance
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
//#include "Hcsr04.h"

int16_t Set_Data,Set_Speed=20, Num;//Set_Speed 是默认的速度，基于摇杆角度，设定电机按照该速度百分比旋转，单位是cm/s
uint8_t Flag, Hcsr04_StartFlag,LINK_FLAG, Error = 0;
uint8_t Info1, Info2, Info3, Info4, State0;
uint8_t X, Y, Value;
int16_t SpeedLeft=0,SpeedRight=0,SpeedRight_Robot=0,SpeedLeft_Robot=0;
int8_t LeftCalibration=1,RightCalibration=-1,LeftInversion=1,RightInversion=1;
uint8_t DataReset=1,StopFlag=0;
uint8_t Left=0,Right=0;
uint8_t State=1;
uint32_t feedTime1, feedTime2, feedTime3;
uint32_t T2Count[3];
void Init(void);
void Speed(int16_t data);
void Data_Analyse(void);
void Get_Distance(void);
void State1(void);
void State2(void);
void State3(void);
void While_Init(void);
void HandleData(void);
	
int main(void)
{	
	Init();	
		//light
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_Init(GPIOA, &GPIO_InitStructure);		

		OLED_ShowString(3, 1, "1:");
		OLED_ShowString(3, 6, "2:");
		OLED_ShowString(3, 12, "3:");
	while (1)
	{
		OLED_ShowHexNum(1,12,Value,4);  //遥杆坐标
		OLED_ShowNum(2,1,feedTime1,4);
		OLED_ShowNum(2,6,feedTime2,4);
		OLED_ShowNum(2,11,feedTime3,4);
		
		MonitorFeed();
			
		While_Init();
		if(Left >=2)
			Left--;
		if(Right >=2)
			Right--;
		switch (State)//遥控右上角拨杆档位
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
		
		HandleData();
		if(StopFlag==SET ){SpeedLeft=0;SpeedRight=0;}
		

/*********************算法驱动************************************/
		SpeedLeft_Robot=LeftInversion*LeftCalibration*SpeedConversion(SpeedLeft);
		SpeedRight_Robot=RightInversion*RightCalibration*SpeedConversion(SpeedRight);

			Robot_Move(SpeedLeft_Robot,SpeedRight_Robot);




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
	LED_Init();
	OLED_Init( );
	NRF24L01_Init();
	RX_Mode();
	Hcsr04_Init();
	TIM2_Init();
	USART_Config();
	Feeding_Init();	
	Robot_Init();
}
void While_Init()
{
	Get_BirdNum();
	Get_Distance();
	StartFeed(&feedTime1, &feedTime2, &feedTime3);
	//		OLED_ShowNum(1,14,State,2); 
	//		Num ++;
	//		OLED_ShowNum(1, 16, Num, 1);
	State0 = NRF24L01_ReadByte(STATUS);
	Info1 = NRF24L01_ReadByte(EN_AA);
	Info2 = NRF24L01_ReadByte(EN_RXADDR);
	Info3 = NRF24L01_ReadByte(RX_ADDR_P0);
	Info4 = NRF24L01_ReadByte(FIFO_STATUS);
	Flag = NRF24L01_ReceiveData();
	//light
	if (Info4 != 0x11)
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	else
		GPIO_SetBits(GPIOA, GPIO_Pin_8);

	OLED_ShowHexNum(4, 1, State0, 2);
	OLED_ShowHexNum(4, 4, Info1, 2);
	OLED_ShowHexNum(4, 7, Info2, 2);
	OLED_ShowHexNum(4, 10, Info3, 2);
	OLED_ShowHexNum(4, 13, Info4, 2);

	if (NRF24L01_GetData(NORMAL_TRANSMIT) != 0)
	OLED_ShowHexNum(1,1,NRF24L01_GetData(NORMAL_TRANSMIT),2);

	if (Flag)
		Flag = 0;
	Delay_ms(8);
	NRF24L01_GetData(NORMAL_TRANSMIT);
	NRF24L01_GetData(KEY_TRANSMIT);		
	NRF24L01_GetData(SWITCH_TRANSMIT);	
	NRF24L01_GetData(ROCKER_TRANSMIT)	;
	NRF24L01_GetData(ENCODER_TRANSMIT);	
	Data_Analyse();
		
		
}

void State1(void)
{
		if(NRF24L01_GetData(SWITCH_TRANSMIT)==SWITCH3_PIN2_NUM+SWITCH2_PIN1_NUM)	//拨钮开关05启用普通按键控制
	{
	/*********************摇杆检测************************************/
		//根据遥杠调整电机速度
		NRF24L01_GetData(ROCKER_TRANSMIT);

		switch(Y)
		{
			case 0x09:Speed(0x00);break;
			case UP_1:Speed(Set_Speed/10);break;
			case UP_2:Speed(Set_Speed/4);break;
			case UP_3:Speed(Set_Speed/2);break;
			case UP_4:Speed(Set_Speed);break;
			case DOWN_1:Speed(-Set_Speed/10);break;
			case DOWN_2:Speed(-Set_Speed/4);break;
			case DOWN_3:Speed(-Set_Speed/2);break;
			case DOWN_4:Speed(-Set_Speed);break;
		}
			LeftInversion=1;RightInversion=1;
		switch(X)
		{			
			case LEFT_1:break;
			case LEFT_2:Speed(Set_Speed/10);LeftInversion=-1;break;
			case LEFT_3:Speed(Set_Speed/4);LeftInversion=-1;break;
			case LEFT_4:Speed(Set_Speed/2);LeftInversion=-1;break;
			case RIGHT_1:break;
			case RIGHT_2:Speed(Set_Speed/10);RightInversion=-1;break;
			case RIGHT_3:Speed(Set_Speed/4);RightInversion=-1;break;
			case RIGHT_4:Speed(Set_Speed/2);RightInversion=-1;break;
		
		}
		
		/********************按键检测************************************/
	
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
////			case EmergencyFault	:	SpeedLeft=0;SpeedRight=0;break;//急停
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//启动
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//关闭
			case KEY_PIN3_NUM		:	SpeedLeft=0;SpeedRight=0;break;//急停
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
		
		
		/*********************旋转编码器检测************************************/
	
		if(NRF24L01_GetData(KEY_TRANSMIT)==KEY_PIN4_NUM)
		{
			Set_Data=NRF24L01_GetData(ENCODER_TRANSMIT);
			Speed(Set_Data);
		}

	
/*********************喂料************************************/		
		switch( NRF24L01_GetData(NORMAL_TRANSMIT))
		{
//			case FEED_OFF:	//喂料器关
//				Relay_Set(ALL, RESET);
//			break;
//			case FEED_ON:	//喂料器开
//				Relay_Set(ALL, SET);
//			break;
			case AUTO_FEED:	//自动喂料开
				GetFeedTime(&feedTime1, &feedTime2, &feedTime3); 
			break;
		}
		
		
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault	:	SpeedLeft=0;SpeedRight=0;break;//急停
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//启动
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//关闭
			case KEY_PIN3_NUM		:	SpeedLeft=0;SpeedRight=0;break;//急停
		}
		
		
	}

void State3(void)
{
	switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault	:	SpeedLeft=0;SpeedRight=0;break;//急停
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//启动
			case KEY_PIN2_NUM		:	break;
			case KEY_PIN3_NUM		:	SpeedLeft=0;SpeedRight=0;break;//急停
			case KEY_PIN4_NUM		:	break;
			case KEY_PIN5_NUM		:	break;
			case KEY_PIN6_NUM		:	break;
			case KEY_PIN7_NUM		:	break;
		}
}

void Data_Analyse(void)
{
	static uint8_t SwitchNum, LastSwitchNum;	
	Value = NRF24L01_GetData(ROCKER_TRANSMIT);
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
	static uint8_t LastSwitchNum, NormalNum, LastLinkFlag, NowLinkFlag;
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
void Speed(int16_t data)
{

	SpeedLeft=data;
	SpeedRight=data;
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
	
        //灯闪烁
        T2Count[1] ++;
        if (T2Count[1] >= 500)
        {
            T2Count[1] = 0;
            GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction)!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1));
        }  

        //超时时间
        if (Timeout > 0)
            Timeout --;
		else
			Timeout = 0;
		//喂料器时间
		if (feedTime1 > 0) feedTime1 --;
		else feedTime1 = 0;
		if (feedTime2 > 0) feedTime2 --;
		else feedTime2 = 0;
		if (feedTime3 > 0) feedTime3 --;
		else feedTime3 = 0;
		
	}
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	
}
