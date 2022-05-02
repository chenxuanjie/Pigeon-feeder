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
#include "USART.h"
//#include "Hcsr04.h"

int16_t Set_Data,Set_Speed=20, Num;
uint8_t Flag, Hcsr04_StartFlag;
uint8_t Info1, Info2, Info3, Info4, State0;
uint8_t X, Y, Value;
float Distance1, Distance2, Distance3, Distance4;
int16_t SpeedLeft=0,SpeedRight=0;
int8_t LeftCalibration=1,RightCalibration=-1,LeftInversion=1,RightInversion=1;
uint8_t DataReset=1;
uint8_t State=1;
uint32_t Timeout;

void Speed(int16_t data);
void Data_Analyse(void);
void Distance_Get(void);
void State1(void);
void State2(void);
void State3(void);
void While_Init(void);

int main(void)
{	
	LED_Init();
	OLED_Init( );
	OLED_ShowString(2,1,"1:00.0  2:00.0cm");
	OLED_ShowString(3,1,"3:00.0  4:00.0cm");	
	NRF24L01_Init();
	RX_Mode();
	Relay_Init();
	Hcsr04_Init();
	TIM2_Init();
	USART_Config();
	
	LEDO_OFF();
	
	 Robot_Init();
	
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

	while (1)
	{
			
		While_Init();
		
		switch (State)
		{
			case SETTINGSTATE://下
				State1();
				break;
			case CONTROLSTATE://中
				State2();
				break;
			case DEBUGSTATE://上
				State2();
				break;
		}
		

		
		 DataReset++;
		 DataReset%=20;
	if(DataReset ==0)
		 NRF24L01_DataReset(); //按键信息自动重置，防FNR通讯中断导致失控

/*********************算法驱动************************************/
		SpeedLeft=LeftInversion*LeftCalibration*SpeedConversion(SpeedLeft);
		SpeedRight=RightInversion*RightCalibration*SpeedConversion(SpeedRight);

			Robot_Move(SpeedLeft,SpeedRight);




		LEDO_OFF();
		
		
		
	}
}

void While_Init()
{
	
		Distance_Get();

		Num ++;
		OLED_ShowNum(1, 16, Num, 1);
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
		
		OLED_ShowHexNum(1,1,NRF24L01_GetData(ROCKER_TRANSMIT),2);

		
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
		
		
}

void State1(void)
{
		if(NRF24L01_GetData(SWITCH_TRANSMIT)==SWITCH3_PIN2_NUM+SWITCH2_PIN1_NUM)	//拨钮开关05启用普通按键控制
	{
	/*********************摇杆检测************************************/
	NRF24L01_GetData(ROCKER_TRANSMIT);

		switch(Y)
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
			case EmergencyFault	:	SpeedLeft=0;SpeedRight=0;break;//急停
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
			case KEY_PIN2_NUM		:	Timeout=1000;while(Timeout){};break;     //自动视觉识别并喂料
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
			case FEED_OFF:	//喂料器关
				Relay_Set(ALL, RESET);
			break;
			case FEED_ON:	//喂料器开
				Relay_Set(ALL, SET);
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
	OLED_ShowHexNum(1,4,Hcsr04_StartFlag,2);
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
	OLED_ShowNum(2 , 3, Distance1/10, 2);
	OLED_ShowNum(2 , 11, Distance2/10, 2);
	OLED_ShowNum(3 , 3, Distance3/10, 2);
	OLED_ShowNum(3 , 11, Distance4/10, 2);
	OLED_ShowNum(2 , 6, (uint32_t)Distance1%10 ,1);
	OLED_ShowNum(2 , 14, (uint32_t)Distance2%10 ,1);
	OLED_ShowNum(3 , 6, (uint32_t)Distance3%10 ,1);
	OLED_ShowNum(3 , 14, (uint32_t)Distance4%10 ,1);
//	OLED_ShowString(1, 1, "1:  . cm");
//	OLED_ShowString(2, 1, "2:  . cm");
//	OLED_ShowString(3, 1, "3:  . cm");
//	OLED_ShowString(3, 12, ". cm");

}

void TIM2_IRQHandler(void)
{
	static uint32_t T2Count[3];
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
	{
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

	}
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	
}
