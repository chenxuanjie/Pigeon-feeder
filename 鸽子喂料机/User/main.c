/*
	�����յ������鼰����ֵ�������������main.h��
	
	NRF24L01����Ԫ�أ�
		1.NORMAL_TRANSMIT�����档
		2.KEY_TRANSMIT��������
		3.SWITCH_TRANSMIT��ť�ӿ��ء�
		4.ROCKER_TRANSMIT��ҡ�ˡ�
		5.ENCODER_TRANSMIT����ת��������
*/

/*
Program: pigeon-feeder
History:
	2023/4/14	Shane	16th release
	code refactoring. �����ع�
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

int16_t Set_Data,Set_Speed=20, Num;//Set_Speed ��Ĭ�ϵ��ٶȣ�����ҡ�˽Ƕȣ��趨������ո��ٶȰٷֱ���ת����λ��cm/s
uint8_t Flag, Hcsr04_StartFlag,LINK_FLAG, Error = 0;
uint8_t Info1, Info2, Info3, Info4, State0;
uint8_t X, Y, Value;
float Distance1, Distance2, Distance3, Distance4;
int16_t SpeedLeft=0,SpeedRight=0,SpeedRight_Robot=0,SpeedLeft_Robot=0;
int8_t LeftCalibration=1,RightCalibration=-1,LeftInversion=1,RightInversion=1;
uint8_t DataReset=1,StopFlag=0;
uint8_t Left=0,Right=0;
uint8_t State=1;
uint32_t feedTime1, feedTime2, feedTime3;
uint32_t T2Count[3];
void Speed(int16_t data);
void Data_Analyse(void);
void Distance_Get(void);
void State1(void);
void State2(void);
void State3(void);
void While_Init(void);
void GetBirdNum(void);
void HandleData(void);
	
int main(void)
{	
	LED_Init();
	OLED_Init( );
//	OLED_ShowString(2,1,"1:00.0  2:00.0cm");
//	OLED_ShowString(3,1,"3:00.0  4:00.0cm");	
	NRF24L01_Init();
	RX_Mode();
	Relay_Init();
	Hcsr04_Init();
	TIM2_Init();
	USART_Config();
	Feed_Init();
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

		OLED_ShowString(3, 1, "1:");
		OLED_ShowString(3, 6, "2:");
		OLED_ShowString(3, 12, "3:");
	while (1)
	{
		OLED_ShowHexNum(1,12,Value,4);  //ң������
		OLED_ShowNum(2,1,feedTime1,4);
		OLED_ShowNum(2,6,feedTime2,4);
		OLED_ShowNum(2,11,feedTime3,4);

//���ʣ������
//1	
		if(Distance1 > 169.7)
		{
			OLED_ShowString(3, 3, "Lo");
		}
		else
		{	
			if(Distance1 > 62.35 && Distance1 <= 169.7)
			{
				OLED_ShowString(3, 3, "Mi");
			}
			
			else
			{
				OLED_ShowString(3, 3, "Fu");
			}
			
		}

//2
		if(Distance2 > 169.7)
		{
			OLED_ShowString(3, 8, "Lo");
		}
		else
		{	
			if(Distance2 > 62.35 && Distance2 <= 169.7)
			{
				OLED_ShowString(3, 8, "Mi");
			}
			
			else
			{
				OLED_ShowString(3, 8, "Fu");
			}
			
		}
		
//3
		if(Distance3 > 169.7)
		{
			OLED_ShowString(3, 14, "Lo");
		}
		else
		{	
			if(Distance3 > 62.35 && Distance3 <= 169.7)
			{
				OLED_ShowString(3, 14, "Mi");
			}
			
			else
			{
				OLED_ShowString(3, 14, "Fu");
			}
			
		}
		
		

//		OLED_ShowNum(3 , 1, Distance1/10, 2);
////		OLED_ShowString(3, 3,"%");
//		OLED_ShowNum(3 , 5, Distance2/10, 2);
////		OLED_ShowString(3, 7,"%");
//		OLED_ShowNum(3 , 9, Distance3/10, 2);
////		OLED_ShowString(3, 11,"%");
////		OLED_ShowNum(3 , 13, Distance4/10, 2);//������
	
		While_Init();
		if(Left >=2)
			Left--;
		if(Right >=2)
			Right--;
		switch (State)//ң�����Ͻǲ��˵�λ
		{
			case SETTINGSTATE://��
				State3();
				break;
			case CONTROLSTATE://��
				State2();
				break;
			case DEBUGSTATE://��
				State1();
				break;
		}
		
		HandleData();
		if(StopFlag==SET ){SpeedLeft=0;SpeedRight=0;}
		

/*********************�㷨����************************************/
		SpeedLeft_Robot=LeftInversion*LeftCalibration*SpeedConversion(SpeedLeft);
		SpeedRight_Robot=RightInversion*RightCalibration*SpeedConversion(SpeedRight);

			Robot_Move(SpeedLeft_Robot,SpeedRight_Robot);




		LEDO_OFF();
				
	}
}

void While_Init()
{
		GetBirdNum();
		Distance_Get();
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
		if(NRF24L01_GetData(SWITCH_TRANSMIT)==SWITCH3_PIN2_NUM+SWITCH2_PIN1_NUM)	//��ť����05������ͨ��������
	{
	/*********************ҡ�˼��************************************/
		//����ң�ܵ�������ٶ�
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
		
		/********************�������************************************/
	
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
////			case EmergencyFault	:	SpeedLeft=0;SpeedRight=0;break;//��ͣ
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//����
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//�ر�
			case KEY_PIN3_NUM		:	SpeedLeft=0;SpeedRight=0;break;//��ͣ
			case KEY_PIN4_NUM		:	break;//ʹ����ת������
			case KEY_PIN5_NUM		:	Set_Speed=NRF24L01_GetData(ENCODER_TRANSMIT);break;
			case KEY_PIN6_NUM		:	break;
			case KEY_PIN7_NUM		:	break;
		
		}
	}
}

void State2(void)
{
	
	
		
		/* �л�����ģʽʱ���ǵð�����������ֹͣ�����������л���ͨģʽ���±�����һ�ΰ�������*/
	if(NRF24L01_GetData(SWITCH_TRANSMIT)==SWITCH3_PIN2_NUM)//��ť����04����У׼ģʽ,У׼�������ҡ�˿���	
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case KEY_PIN4_NUM		:	LeftCalibration=-1;RightCalibration=1;break;
			case KEY_PIN5_NUM		:	LeftCalibration=1;RightCalibration=-1;break;
			case KEY_PIN6_NUM		:	LeftCalibration=1;RightCalibration=1;break;
			case KEY_PIN7_NUM		:	LeftCalibration=-1;RightCalibration=-1;break;
		}
		
		
		/*********************��ת���������************************************/
	
		if(NRF24L01_GetData(KEY_TRANSMIT)==KEY_PIN4_NUM)
		{
			Set_Data=NRF24L01_GetData(ENCODER_TRANSMIT);
			Speed(Set_Data);
		}

	
/*********************ι��************************************/		
		switch( NRF24L01_GetData(NORMAL_TRANSMIT))
		{
//			case FEED_OFF:	//ι������
//				Relay_Set(ALL, RESET);
//			break;
//			case FEED_ON:	//ι������
//				Relay_Set(ALL, SET);
//			break;
			case AUTO_FEED:	//�Զ�ι�Ͽ�
				GetFeedTime(&feedTime1, &feedTime2, &feedTime3); 
			break;
		}
		
		
		switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault	:	SpeedLeft=0;SpeedRight=0;break;//��ͣ
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//����
			case KEY_PIN2_NUM		:	TSDA_Order(LeftWheel,MotorStop);TSDA_Order(RightWheel,MotorStop);break;//�ر�
			case KEY_PIN3_NUM		:	SpeedLeft=0;SpeedRight=0;break;//��ͣ
		}
		
		
	}

void State3(void)
{
	switch(NRF24L01_GetData(KEY_TRANSMIT))
		{
			case EmergencyFault	:	SpeedLeft=0;SpeedRight=0;break;//��ͣ
			case KEY_PIN1_NUM		:	TSDA_Order(LeftWheel,MotorStart);TSDA_Order(RightWheel,MotorStart);break;//����
			case KEY_PIN2_NUM		:	break;
			case KEY_PIN3_NUM		:	SpeedLeft=0;SpeedRight=0;break;//��ͣ
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
	X = Value&0x0F;
	Y = (Value&0xF0)>>4;
	
	LastSwitchNum = SwitchNum;
	//Stateֵ�ĸı�
	SwitchNum = NRF24L01_GetData(SWITCH_TRANSMIT);	
	if (SwitchNum != LastSwitchNum)
	{
		if ((SwitchNum&0x02) != 0)	//����ť�ӿ������²�
		{
			State = SETTINGSTATE;		
		}
		else if ((SwitchNum&0x08) != 0)	//����ť�ӿ������м䲦
		{
			State = CONTROLSTATE;
		}
		else 							//����ť�ӿ������ϲ�
			State = DEBUGSTATE;
	}
}

void HandleData(void)
{
	static uint8_t LastSwitchNum, NormalNum, LastLinkFlag, NowLinkFlag;
	NormalNum = NRF24L01_GetData(NORMAL_TRANSMIT);

	//�Ͽ����ӵ����
	LastLinkFlag = NowLinkFlag;
	NowLinkFlag = LINK_FLAG;
	if (LastLinkFlag==0 && NowLinkFlag==1)
	StopFlag=RESET;
	else if (LastLinkFlag==1 && NowLinkFlag==0) //ͨѶ�Ͽ�
	{StopFlag=SET;
	}
	//���淢��λ���λ��Ϊ���ӱ�־λ	
	if ((NormalNum&0x80)!=RESET && Error==SET)	//��������λû�б����
	{

		LINK_FLAG = 0;
	}
	else if ((NormalNum&0x80)==RESET && Error==SET)//��������
	{

		LINK_FLAG = 1;
		Error = RESET;		//���Ǵ������
		T2Count[2] = 0;		
		NRF24L01_SetRXBuf(NORMAL_TRANSMIT, (NormalNum |= 0x80));	//��λ��־λ�����´��Ƿ����

	}
	
}
void Speed(int16_t data)
{

	SpeedLeft=data;
	SpeedRight=data;
}

void Distance_Get(void)
{
	if (Hcsr04_StartFlag&0x08)	//�������Ƿ�ʹ��
	{
		switch(Hcsr04_StartFlag&0x07)	//�ڼ�������������
		{
			case 1: Distance1 = Hcsr04_GetDistance1();break;
			case 2: Distance2 = Hcsr04_GetDistance2();break;
			case 3: Distance3 = Hcsr04_GetDistance3();break;
			case 4: Distance4 = Hcsr04_GetDistance4();break;						
		}
	Hcsr04_StartFlag &= 0x07;		//���ʹ��λ
	}	
	//show
//	OLED_ShowNum(2 , 3, Distance1/10, 2);
//	OLED_ShowNum(2 , 11, Distance2/10, 2);
//	OLED_ShowNum(3 , 3, Distance3/10, 2);
//	OLED_ShowNum(3 , 11, Distance4/10, 2);
//	OLED_ShowNum(2 , 6, (uint32_t)Distance1%10 ,1);
//	OLED_ShowNum(2 , 14, (uint32_t)Distance2%10 ,1);
//	OLED_ShowNum(3 , 6, (uint32_t)Distance3%10 ,1);
//	OLED_ShowNum(3 , 14, (uint32_t)Distance4%10 ,1);
}




void TIM2_IRQHandler(void)
{

	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
	{
			//0.5s����Ӧ����Ͽ�����
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
			if ((Hcsr04_StartFlag&0x07) < 4)	//С��4����
				Hcsr04_StartFlag ++;
			else 
				Hcsr04_StartFlag = 0x01;		//����4����Ϊ1
			Hcsr04_StartFlag |= 0x08;			//���������ʹ��λ
		}
	
        //����˸
        T2Count[1] ++;
        if (T2Count[1] >= 500)
        {
            T2Count[1] = 0;
            GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction)!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1));
        }  

        //��ʱʱ��
        if (Timeout > 0)
            Timeout --;
		else
			Timeout = 0;
		//ι����ʱ��
		if (feedTime1 > 0) feedTime1 --;
		else feedTime1 = 0;
		if (feedTime2 > 0) feedTime2 --;
		else feedTime2 = 0;
		if (feedTime3 > 0) feedTime3 --;
		else feedTime3 = 0;
		
	}
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	
}
