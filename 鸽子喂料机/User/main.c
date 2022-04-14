/*
	△接收到的数组及其数值（具体参数都在main.h）
	
	NRF24L01数组元素：
		1.NORMAL_TRANSMIT：常规。
		2.KEY_TRANSMIT：按键。
		3.SWITCH_TRANSMIT：钮子开关。
		4.ROCKER_TRANSMIT：摇杆。
		5.ENCODER_TRANSMIT：旋转编码器。
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
//#include "Hcsr04.h"

int16_t Set_Data,Set_Speed=20, Num;
uint8_t Flag, Hcsr04_StartFlag;
uint8_t Info1, Info2, Info3, Info4, State1;
uint8_t X, Y, Value;
uint32_t Distance1, Distance2, Distance3, Distance4;
int16_t SpeedLeft=0,SpeedRight=0;
int8_t LeftCalibration=1,RightCalibration=1,LeftInversion=1,RightInversion=1;


void Speed(int16_t data);
void Data_Analyse(void);
void Distance_Get(void);

int main(void)
{	
	LED_Init();
	OLED_Init( );
//	Delay_ms(100);
	NRF24L01_Init();
	RX_Mode();
	Relay_Init();
	Hcsr04_Init();
	TIM2_Init();
	OLED_ShowString(1, 1, "1:  . cm");
	OLED_ShowString(2, 1, "2:  . cm");
	OLED_ShowString(3, 1, "3:  . cm");
	OLED_ShowString(3, 12, ". cm");
	USART_Config();
	
	LEDO_OFF();
	
	 Robot_Init();
	while (1)
	{
		Distance_Get();

		Num ++;
		OLED_ShowNum(1, 14, Num, 3);
		State1 = NRF24L01_ReadByte(STATUS);
		Info1 = NRF24L01_ReadByte(EN_AA);
		Info2 = NRF24L01_ReadByte(EN_RXADDR);
		Info3 = NRF24L01_ReadByte(RX_ADDR_P0);
		Info4 = NRF24L01_ReadByte(FIFO_STATUS);
		Flag = NRF24L01_ReceiveData();
		OLED_ShowHexNum(4, 1, State1, 2);
		OLED_ShowHexNum(4, 4, Info1, 2);
		OLED_ShowHexNum(4, 7, Info2, 2);
		OLED_ShowHexNum(4, 10, Info3, 2);
		OLED_ShowHexNum(4, 13, Info4, 2);
		
		if (NRF24L01_GetData(NORMAL_TRANSMIT)!=0)
			OLED_ShowNum(2, 1, NRF24L01_GetData(NORMAL_TRANSMIT), 4);

		
		if (Flag)
		{
			
			Flag = 0;
			
		}
		Delay_ms(8);
			NRF24L01_GetData(NORMAL_TRANSMIT);
			NRF24L01_GetData(KEY_TRANSMIT);		
			NRF24L01_GetData(SWITCH_TRANSMIT);	
			NRF24L01_GetData(ROCKER_TRANSMIT)	;
			NRF24L01_GetData(ENCODER_TRANSMIT);	
		Data_Analyse();
		
		
		
/*********************状态1：摇杆检测************************************/		
		NRF24L01_GetData(ROCKER_TRANSMIT);

		switch(X)
		{
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
		switch(Y)
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

		
/*********************状态0：常规检测************************************/		
		switch( NRF24L01_GetData(NORMAL_TRANSMIT))
		{
			case FEED_OFF:	//喂料器关
				Relay_Set(ALL, RESET);
			break;
			case FEED_ON:	//喂料器开
				Relay_Set(ALL, SET);
			break;
		}
			
		
/*********************状态2：拨钮开关检测************************************/		
		
			switch( NRF24L01_GetData(SWITCH_TRANSMIT))
		{
			case SWITCH3_PIN1_NUM:LeftCalibration=-1;RightCalibration=1;break;
			case SWITCH3_PIN2_NUM:LeftCalibration=1;RightCalibration=-1;break;
			case SWITCH3_NONE:LeftCalibration=1;RightCalibration=1;break;
			case SWITCH3_NONE+SWITCH2_PIN1_NUM:LeftCalibration=-1;RightCalibration=-1;break;
				
		}
		
/*********************状态3：旋转编码器检测************************************/		
		if(NRF24L01_GetData(KEY_TRANSMIT)==KEY_PIN4_NUM)
		{
			Set_Data=NRF24L01_GetData(ENCODER_TRANSMIT);
			Speed(Set_Data);
		}
		
/*********************状态4：按键检测************************************/
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault	:	SpeedLeft=0;SpeedRight=0;break;//急停
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//启动
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//关闭
			case KEY_PIN3_NUM		:	SpeedLeft=0;SpeedRight=0;break;//急停
			case KEY_PIN4_NUM		:	break;//使用旋转编码器
			case KEY_PIN5_NUM		:	Set_Speed=NRF24L01_GetData(ENCODER_TRANSMIT);break;
			case KEY_PIN6_NUM		:	break;
			case KEY_PIN7_NUM		:	break;
		
		}

		 NRF24L01_DataReset(); //按键信息自动重置，防FNR通讯中断导致失控

/*********************状态5：算法驱动************************************/
		SpeedLeft=LeftInversion*LeftCalibration*SpeedConversion(SpeedLeft);
		SpeedRight=RightInversion*RightCalibration*SpeedConversion(SpeedRight);

			Robot_Move(SpeedLeft,SpeedRight);
//			TSDA_Data(LeftWheel,SpeedSetting,SpeedLeft);
//		
//			TSDA_Data(LeftWheel,SpeedSetting,SpeedLeft);		
//			TSDA_Data(RightWheel,SpeedSetting,SpeedRight);

//				Delay_ms(1000);
		
		
		LEDO_OFF();
		
		
		
	}
}

void Data_Analyse(void)
{
	
	Value = NRF24L01_GetData(ROCKER_TRANSMIT);
	X = Value&0x0F;
	Y = (Value&0xF0)>>4;
}
void Speed(int16_t data)
{

	SpeedLeft=data;
	SpeedRight=data;
}

void Distance_Get(void)
{
	if (Hcsr04_StartFlag&0x08)	//超声波是否使能
	{
		switch(Hcsr04_StartFlag&0x07)	//第几个超声波运行
		{
			case 1: Distance1 = Hcsr04_GetDistance1();break;
			case 2: Distance2 = Hcsr04_GetDistance2();break;
			case 3: Distance3 = Hcsr04_GetDistance3();break;
			case 4: Distance4 = Hcsr04_GetDistance4();break;						
		}
	Hcsr04_StartFlag &= 0x07;		//清除使能位
	}	
	//show
	OLED_ShowNum(1 , 3, Distance1/10 ,2);
	OLED_ShowNum(2 , 3, Distance2/10 ,2);
	OLED_ShowNum(3 , 3, Distance3/10 ,2);
	OLED_ShowNum(3 , 10, Distance4/10 ,2);
	OLED_ShowNum(1 , 6, Distance1%10 ,1);
	OLED_ShowNum(2 , 6, Distance2%10 ,1);
	OLED_ShowNum(3 , 6, Distance3%10 ,1);
	OLED_ShowNum(3 , 13, Distance4%10 ,1);
}

void TIM2_IRQHandler(void)
{
	static uint32_t T2Count;
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
	{
		T2Count++;
		if (T2Count >= 100)
		{
			T2Count = 0;
			if ((Hcsr04_StartFlag&0x07) <= 4)	//小于4，加
				Hcsr04_StartFlag ++;
			else 
				Hcsr04_StartFlag = 0x01;		//大于4，变为1
			Hcsr04_StartFlag |= 0x08;			//超声波检测使能位
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	
}
